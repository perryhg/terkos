#include "qeaudio.h"
#include "timeutil.h"
#include <assert.h>

//Maximum number of command error codes to keep a history of
#define QEAUDIO_MAX_ERR_CODES                      10

//Maximum number of commands (not including {en,dis}able) to allow in queue
#define QEAUDIO_MAX_QUEUE_ITEMS                    10

//Maximum size of commands (including command struct but not including
//  {en,dis}able) to allow in queue, in bytes
#define QEAUDIO_MAX_QUEUE_SIZE                     (2*1024*1024)

//Mutex order
//  m_mutex (acquire first)
//  m_playmutex

CQEAudioController::CQEAudioController()
{
    m_pQwerk = CQwerkHardware::GetObject();

    // initialize mutices
    pthread_mutex_init(&m_mutex, NULL);
    pthread_mutex_init(&m_playmutex, NULL);

    // initialize cond variables
    pthread_cond_init(&m_notemptycond, NULL);
    pthread_cond_init(&m_notfullcond, NULL);

    m_threadRun = true;

    next_seqnum = 1;
    audioCommands_size = 0;
    enabled = false;

    pthread_create(&m_thread, NULL, AudioThread, (void *)this);
}

CQEAudioController::~CQEAudioController()
{
    // signal thread
    pthread_mutex_lock(&m_mutex);

    m_threadRun = false;
    pthread_cond_signal(&m_notemptycond);

    //Don't need to signal m_notfullcond because we assume that no further
    //  user calls are being made (we couldn't destruct safely otherwise)

    pthread_mutex_unlock(&m_mutex);

    // wait for it to exit
    pthread_join(m_thread, NULL);

    //Flush any remaining commands
    emptyAudioCommandQueue();

    CQwerkHardware::ReleaseObject();

    pthread_cond_destroy(&m_notemptycond);
    pthread_cond_destroy(&m_notfullcond);
    pthread_mutex_destroy(&m_mutex);
    pthread_mutex_destroy(&m_playmutex);
}

void *CQEAudioController::AudioThread(void *arg)
{
    CQEAudioController *audio = (CQEAudioController*)arg;
    CQEAudioControllerCommand command;
    int ret;
    unsigned int seqnum;
    std::map<unsigned int,int>::iterator err_codes_iter;
    int commandsize;

    pthread_mutex_lock(&audio->m_mutex);
    while (audio->m_threadRun) {
      if (audio->audioCommands.size() > 0) {
        command = audio->audioCommands.front();
        pthread_mutex_unlock(&audio->m_mutex);

        ret = audio->executeAudioCommand(command);

        seqnum = command.seqnum;
        commandsize = getCommandSize(command);

        pthread_mutex_lock(&audio->m_mutex);

        audio->audioCommands.pop();
        audio->audioCommands_size -= commandsize;

        //add seqnum to err_codes
        audio->err_codes[seqnum] = ret;

        //delete seqnum-QEAUDIO_MAX_ERR_CODES from err_codes
        err_codes_iter = audio->err_codes.find(seqnum-QEAUDIO_MAX_ERR_CODES);
        if (err_codes_iter != audio->err_codes.end())
          audio->err_codes.erase(err_codes_iter);

        assert(audio->err_codes.size() <= QEAUDIO_MAX_ERR_CODES);

        pthread_cond_broadcast(&audio->m_notfullcond);

        pthread_mutex_unlock(&audio->m_mutex);

        usleep(1000);
        pthread_mutex_lock(&audio->m_mutex);
      }
      else
        pthread_cond_wait(&audio->m_notemptycond, &audio->m_mutex);
    }
    pthread_mutex_unlock(&audio->m_mutex);

    return NULL;
}

int CQEAudioController::getErrorCode(unsigned int seqnum, unsigned int maxwait)
{
    int err;
    struct timespec ts;
    std::map<unsigned int,int>::iterator err_codes_iter;

    pthread_mutex_lock(&m_mutex);

    //See if error code already available
    err_codes_iter = err_codes.find(seqnum);
    if (err_codes_iter != err_codes.end()) {
        //It is
        err = err_codes[seqnum];
        pthread_mutex_unlock(&m_mutex);
        return err;
    }
    //Otherwise, it isn't -- we'd need to wait

    if (maxwait <= 0) {
        pthread_mutex_unlock(&m_mutex);
        return QEAUDIO_RETURN_UNAVAILABLE;
    }

    TIMESPEC_SET_ADD_MSEC(&ts, maxwait);
    pthread_cond_timedwait(&m_notfullcond, &m_mutex, &ts);

    err_codes_iter = err_codes.find(seqnum);
    if (err_codes_iter != err_codes.end())
        err = err_codes[seqnum];
    else
        err = QEAUDIO_RETURN_UNAVAILABLE;

    pthread_mutex_unlock(&m_mutex);

    return err;
}

void CQEAudioController::enable()
{
    pthread_mutex_lock(&m_mutex);
    if (!enabled) {
        doEnable();
        enabled = true;
    }
    pthread_mutex_unlock(&m_mutex);
}

void CQEAudioController::disable()
{
    pthread_mutex_lock(&m_mutex);
    if (enabled) {
        doDisable();
        enabled = false;
    }
    pthread_mutex_unlock(&m_mutex);
}

void CQEAudioController::doEnable()
{
    m_pQwerk->SetAudioEnable(true);
    *(m_pQwerk->m_p9302hw)->m_fpga.Ushort(QHW_AUDIO_VOLUME) = 0;

    //If experiencing problems with audio being cut short at start,
    //  try inserting usleep(100000) here
}

void CQEAudioController::doDisable()
{
    *(m_pQwerk->m_p9302hw)->m_fpga.Ushort(QHW_AUDIO_VOLUME) = 0;
    m_pQwerk->SetAudioEnable(false);
}

//m_mutex must not be held when called
int CQEAudioController::executeAudioCommand(CQEAudioControllerCommand &command)
{
  int ret;

  printf("Executing audio command: seq=%u, type=%d: starting\n", command.seqnum, command.type);

  pthread_mutex_lock(&m_playmutex);
  switch (command.type) {
    case AUDIOCOMMAND_TYPE_TONE:
      enable();
      ret = doPlayTone(m_pQwerk, command.data.tone.frequency, command.data.tone.amplitude, command.data.tone.duration);
      break;
    case AUDIOCOMMAND_TYPE_CLIP:
      enable();
      ret = doPlayClip(command.data.clip.clip, command.data.clip.clipLength);
      free(command.data.clip.clip);
      break;
    case AUDIOCOMMAND_TYPE_ENABLE:
      enable();
      ret = QEAUDIO_RETURN_OK;
      break;
    case AUDIOCOMMAND_TYPE_DISABLE:
      disable();
      ret = QEAUDIO_RETURN_OK;
      break;
    default:
      ret = QEAUDIO_ERROR;
      assert(0);
      break;
  }
  pthread_mutex_unlock(&m_playmutex);

  printf("Executing audio command: seq=%u, type=%d: returning %d\n", command.seqnum, command.type, ret);

  return ret;
}

int CQEAudioController::enqueueTone(long frequency, int amplitude, long duration, unsigned int &seqnum, unsigned int maxwait)
{
    CQEAudioControllerCommand command;

    command.type = AUDIOCOMMAND_TYPE_TONE;
    command.data.tone.frequency = frequency;
    command.data.tone.amplitude = amplitude;
    command.data.tone.duration  = duration;

    return enqueueCommand(command, seqnum, maxwait);
}

int CQEAudioController::enqueueClip(std::vector<unsigned char> clip, unsigned int &seqnum, unsigned int maxwait)
{
    CQEAudioControllerCommand command;
    int ret;
    int i;
    int length = clip.size();

    command.type = AUDIOCOMMAND_TYPE_CLIP;
    command.data.clip.clip = (char*)malloc(length);
    if (command.data.clip.clip == NULL)
      return QEAUDIO_RETURN_ALLOC_ERROR;
    for (i = 0; i < length; i++)
      command.data.clip.clip[i] = clip.at(i);
    command.data.clip.clipLength = length;

    ret = enqueueCommand(command, seqnum, maxwait);
    if (ret != QEAUDIO_RETURN_OK)
      free(command.data.clip.clip);

    return ret;
}

int CQEAudioController::enqueueClip(char clip[], int length, unsigned int &seqnum, unsigned int maxwait)
{
    CQEAudioControllerCommand command;
    int ret;

    command.type = AUDIOCOMMAND_TYPE_CLIP;
    command.data.clip.clip = (char*)malloc(length);
    if (command.data.clip.clip == NULL)
      return QEAUDIO_RETURN_ALLOC_ERROR;
    memcpy(command.data.clip.clip, clip, length);
    command.data.clip.clipLength = length;

    ret = enqueueCommand(command, seqnum, maxwait);
    if (ret != QEAUDIO_RETURN_OK)
      free(command.data.clip.clip);

    return ret;
}

int CQEAudioController::enqueueEnable(void)
{
    CQEAudioControllerCommand command;
    unsigned int seqnum;

    command.type = AUDIOCOMMAND_TYPE_ENABLE;

    return enqueueCommand(command, seqnum, 0);
}

int CQEAudioController::enqueueDisable(void)
{
    CQEAudioControllerCommand command;
    unsigned int seqnum;

    command.type = AUDIOCOMMAND_TYPE_DISABLE;

    return enqueueCommand(command, seqnum, 0);
}

int CQEAudioController::enqueueCommand(CQEAudioControllerCommand &command, unsigned int &seqnum, unsigned int maxwait)
{
    int commandsize;
    struct timespec ts;

    commandsize = getCommandSize(command);
    if (commandsize > QEAUDIO_MAX_QUEUE_SIZE)
      return QEAUDIO_RETURN_TOO_LARGE;

    pthread_mutex_lock(&m_mutex);

    if (command.type == AUDIOCOMMAND_TYPE_ENABLE ||
        command.type == AUDIOCOMMAND_TYPE_DISABLE) {
      if (audioCommands.size() > 0) {
        CQEAudioControllerCommand &lastcommand = audioCommands.back();
        if (lastcommand.type == command.type) {
          pthread_mutex_unlock(&m_mutex);
          return QEAUDIO_RETURN_OK;
        }
      }

      command.seqnum = 0;
      audioCommands.push(command);
      audioCommands_size += commandsize;

      pthread_cond_broadcast(&m_notemptycond);
      pthread_mutex_unlock(&m_mutex);

      return QEAUDIO_RETURN_OK;
    }

    TIMESPEC_SET_ADD_MSEC(&ts, maxwait);
    while (audioCommands.size() >= QEAUDIO_MAX_QUEUE_ITEMS ||
           audioCommands_size + commandsize > QEAUDIO_MAX_QUEUE_SIZE) {
        if (pthread_cond_timedwait(&m_notfullcond, &m_mutex, &ts) == ETIMEDOUT) {
            pthread_mutex_unlock(&m_mutex);
            return QEAUDIO_RETURN_QUEUE_FULL;
        }
    }

    command.seqnum = seqnum = next_seqnum++;
    audioCommands.push(command);
    audioCommands_size += commandsize;

    pthread_cond_broadcast(&m_notemptycond);

    pthread_mutex_unlock(&m_mutex);

    return QEAUDIO_RETURN_OK;
}

int CQEAudioController::playTone(long frequency, int amplitude, long duration)
{
  int ret;

  pthread_mutex_lock(&m_playmutex);
  ret = doPlayTone(m_pQwerk, frequency, amplitude, duration);
  pthread_mutex_unlock(&m_playmutex);

  return ret;
}

int CQEAudioController::doPlayTone(CQwerkHardware *qwerk, long frequency, int amplitude, long duration)
{
    *(qwerk->m_p9302hw)->m_fpga.Ushort(QHW_AUDIO_PERIOD) = 1562500/frequency;
    *(qwerk->m_p9302hw)->m_fpga.Ushort(QHW_AUDIO_VOLUME) = amplitude;
    usleep(duration*1000);
    *(qwerk->m_p9302hw)->m_fpga.Ushort(QHW_AUDIO_VOLUME) = 0;

    return QEAUDIO_RETURN_OK;
}

int CQEAudioController::playClip(char clip[], int length)
{
  int ret;

  pthread_mutex_lock(&m_playmutex);
  ret = doPlayClip(clip, length);
  pthread_mutex_unlock(&m_playmutex);

  return ret;
}

int CQEAudioController::doPlayClip(char clip[], int length)
{
    FILE *out;

    int nbytes, bytesleft, byteswritten;

    out = popen("/usr/bin/aplay", "w");
    if (!out)
    {
        printf("Unable to run aplay: %s\n", strerror(errno));
        return QEAUDIO_RETURN_UNABLE_TO_RUN_APLAY;
    }

    bytesleft = length;
    nbytes = length;

    while (bytesleft > 0)
    {
        byteswritten = fwrite(clip+nbytes-bytesleft, 1, bytesleft, out);
        if (byteswritten < 0 || ferror(out))
        {
            printf("Error writing to pipe: %s\n", strerror(errno));
            pclose(out);
            return QEAUDIO_RETURN_ERROR_WRITING_TO_PIPE;
        }
        bytesleft -= byteswritten;
    }

    fflush(out);
    if (pclose(out) < 0)
    {
        printf("Warning: error closing pipe: %s\n", strerror(errno));
        return QEAUDIO_RETURN_ERROR_CLOSING_PIPE;
    }

    return QEAUDIO_RETURN_OK;
}

int CQEAudioController::playClip(char *filename)
{
  int ret;

  pthread_mutex_lock(&m_playmutex);
  ret = doPlayClip(filename);
  pthread_mutex_unlock(&m_playmutex);

  return ret;
}

int CQEAudioController::doPlayClip(char *filename)
{
  char buf[0x100];
  FILE *clip, *out;
  int read, written;

  clip = fopen(filename, "r");

  out = popen("/usr/bin/aplay", "w");
  if (!out)
    {
        printf("Unable to run aplay: %s\n", strerror(errno));
        return QEAUDIO_RETURN_UNABLE_TO_RUN_APLAY;
    }
  if (clip==NULL)
    {
    printf("Unable to run open file: %s\n", strerror(errno));
    return QEAUDIO_ERROR;
    }

  while (1)
    {
      read = fread(buf, 1, 0x100, clip);
      if (read==0)
	break;
      written = fwrite(buf, 1, read, out);
      if (written < 0 || ferror(out))
        {
	  printf("Error writing to pipe: %s\n", strerror(errno));
	  pclose(out);
	  return QEAUDIO_RETURN_ERROR_WRITING_TO_PIPE;
	}

    }

  fflush(out);

  fclose(clip);
  pclose(out);

  return QEAUDIO_RETURN_OK;
}

int CQEAudioController::getCommandSize(CQEAudioControllerCommand &command)
{
  switch (command.type) {
    case AUDIOCOMMAND_TYPE_ENABLE:
    case AUDIOCOMMAND_TYPE_DISABLE:
    case AUDIOCOMMAND_TYPE_TONE:
      return sizeof(command);
      break;
    case AUDIOCOMMAND_TYPE_CLIP:
      return sizeof(command) + command.data.clip.clipLength;
      break;
    default:
      assert(0);
      return -1;
      break;
  }
}


void CQEAudioController::emptyAudioCommandQueue(void)
{
  while (!audioCommands.empty()) {
    CQEAudioControllerCommand &command = audioCommands.front();

    switch (command.type) {
      case AUDIOCOMMAND_TYPE_ENABLE:
      case AUDIOCOMMAND_TYPE_DISABLE:
      case AUDIOCOMMAND_TYPE_TONE:
        //nothing to do
        break;
      case AUDIOCOMMAND_TYPE_CLIP:
        free(command.data.clip.clip);
        break;
      default:
        assert(0);
        break;
    }

    audioCommands.pop();
  }
}

