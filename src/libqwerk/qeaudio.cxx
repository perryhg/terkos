#include "qeaudio.h"

SINGLETON_REGISTER(CQEAudioController);

CQEAudioController::CQEAudioController()
{
  m_p9302hw = C9302Hardware::GetPtr();
  if (m_p9302hw->GetBitstreamMajorVersion()!=0xa0)
    throw std::runtime_error("wrong FPGA bitstream version");
  m_config = m_p9302hw->m_fpga.Ushort(QEAUDIO_DEFAULT_ADDR);
  m_volume = m_config+1;
  m_period = m_config+2;
}

CQEAudioController::~CQEAudioController()
{
  *m_config = QEAUDIO_MODE_WAVE;
  C9302Hardware::Release();
}

void CQEAudioController::SetVolume(unsigned char vol)
{
  *m_volume = vol;
}

int CQEAudioController::PlayTone(unsigned short frequency, int duration)
{
  *m_config = QEAUDIO_MODE_TONE;
  *m_period = 1562500/frequency;
  if (duration>0)
    {
      usleep(duration*1000);
      *m_config = QEAUDIO_MODE_WAVE;
    }

  return QEAUDIO_RETURN_OK;
}

int CQEAudioController::PlayClip(const char clip[], int length)
{
  FILE *out;

  int nbytes, bytesleft, byteswritten;

  out = popen("/usr/bin/aplay", "w");
  if (!out)
    {
      printf("Unable to run aplay: %s\n", strerror(errno));
      return QEAUDIO_RETURN_ERROR;
    }

  *m_config = QEAUDIO_MODE_WAVE;
  bytesleft = length;
  nbytes = length;

  while (bytesleft > 0)
    {
      byteswritten = fwrite(clip+nbytes-bytesleft, 1, bytesleft, out);
      if (byteswritten < 0 || ferror(out))
        {
	  printf("Error writing to pipe: %s\n", strerror(errno));
	  pclose(out);
	  return QEAUDIO_RETURN_ERROR;
        }
      bytesleft -= byteswritten;
    }

  fflush(out);
  if (pclose(out) < 0)
    {
      printf("Warning: error closing pipe: %s\n", strerror(errno));
      return QEAUDIO_RETURN_ERROR;
    }

  return QEAUDIO_RETURN_OK;
}

int CQEAudioController::PlayClip(const char *filename)
{
  char buf[0x100];
  FILE *clip, *out;
  int read, written;

  clip = fopen(filename, "r");

  out = popen("/usr/bin/aplay", "w");
  if (!out)
    {
      printf("Unable to run aplay: %s\n", strerror(errno));
      return QEAUDIO_RETURN_ERROR;
    }
  if (clip==NULL)
    {
      printf("Unable to run open file: %s\n", strerror(errno));
      return QEAUDIO_RETURN_ERROR;
    }

  *m_config = QEAUDIO_MODE_WAVE;
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
	  return QEAUDIO_RETURN_ERROR;
	}

    }

  fflush(out);

  fclose(clip);
  pclose(out);

  return QEAUDIO_RETURN_OK;
}

int CQEAudioController::Talk(const char *string)
{
  char buf[0x100];
  FILE *out;

  sprintf(buf, "/usr/bin/flite -t \"%s\"", string); 
  out = popen(buf, "w");
  if (!out)
    {
      printf("Unable to run flite: %s\n", strerror(errno));
      return QEAUDIO_RETURN_ERROR;
    }

  pclose(out);

  return QEAUDIO_RETURN_OK;
}

