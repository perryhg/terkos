#ifndef __QEAUDIO_H__
#define __QEAUDIO_H__

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <queue>
#include <map>

#include "9302hw.h"

#define QEAUDIO_RETURN_UNAVAILABLE                 1
#define QEAUDIO_RETURN_OK                          0
#define QEAUDIO_ERROR                              -1
#define QEAUDIO_RETURN_UNABLE_TO_RUN_APLAY         -2
#define QEAUDIO_RETURN_ERROR_WRITING_TO_PIPE       -3
#define QEAUDIO_RETURN_ERROR_CLOSING_PIPE          -4
#define QEAUDIO_RETURN_QUEUE_FULL                  -5
#define QEAUDIO_RETURN_TOO_LARGE                   -6
#define QEAUDIO_RETURN_ALLOC_ERROR                 -7

#define QEAUDIO_VOLUME              0xfe0
#define QEAUDIO_PERIOD              0xfe2



class CQEAudioController
{
    public:
        CQEAudioController();
        ~CQEAudioController();

        /**
         * @brief Immediately enable audio
         * Should not be called when items may be in the command queue
         * Has no effect if already enabled
         */
	void enable();
        /**
         * @brief Immediately enable audio
         * Should not be called when items may be in the command queue
         * Has no effect if already disabled
         */
	void disable();

        /**
         * @brief Enqueue a tone in the audio command queue
         * @param frequency Frequency of tone (FIXME: units?)
         * @param amplitude Amplitude of tone (FIXME: units?)
         * @param duration Duration of tone (FIXME: units?)
         * @param seqnum Value to be filled in with command sequence number
         * @param maxwait Maximum number of milliseconds to wait if queue full
         * @return QEAUDIO_RETURN_* code
         * Audio will be automatically enabled when this command is executed,
         *   if it was not already enabled
         */
	int enqueueTone(long frequency, int amplitude, long duration, unsigned int &seqnum, unsigned int maxwait);
        /**
         * @brief Enqueue a sound clip in the audio command queue
         * @param clip Clip to enqueue
         * @param seqnum Value to be filled in with command sequence number
         * @param maxwait Maximum number of milliseconds to wait if queue full
         * @return QEAUDIO_RETURN_* code
         * Audio will be automatically enabled when this command is executed,
         *   if it was not already enabled
         */
	int enqueueClip(std::vector<unsigned char> clip, unsigned int &seqnum, unsigned int maxwait);
        /**
         * @brief Enqueue a sound clip in the audio command queue
         * @param clip Clip to enqueue
         * @param length Length of clip (bytes)
         * @param seqnum Value to be filled in with command sequence number
         * @param maxwait Maximum number of milliseconds to wait if queue full
         * @return QEAUDIO_RETURN_* code
         * Audio will be automatically enabled when this command is executed,
         *   if it was not already enabled
         */
	int enqueueClip(char clip[], int length, unsigned int &seqnum, unsigned int maxwait);
        /**
         * @brief Retrieve an error code for a recently enqueued command
         * @param seqnum Sequence number filled in by enqueue method
         * @param maxwait Maximum number of milliseconds to wait if unfinished
         * @return QEAUDIO_RETURN_* code
         */
	int getErrorCode(unsigned int seqnum, unsigned int maxwait);
        /**
         * @brief Enqueue an enabling of audio
         * @return QEAUDIO_RETURN_* code
         * Will have effect if already enabled at time of execution
         */
	int enqueueEnable();
        /**
         * @brief Enqueue a disabling of audio
         * @return QEAUDIO_RETURN_* code
         * Will have effect if already disabled at time of execution
         */
	int enqueueDisable();

        /**
         * @brief Immediately play a tone
         * @param frequency Frequency of tone (FIXME: units?)
         * @param amplitude Amplitude of tone (FIXME: units?)
         * @param duration Duration of tone (FIXME: units?)
         * @return QEAUDIO_RETURN_* code
         * May be called if items in queue
         * Will wait at least for currently executing command or play request
         * If there are any items in queue, will likely (but not assuredly)
         *   be played after currently executing command and before next item
         *   in queue
         */
	int playTone(long frequency, int amplitude, long duration);
        /**
         * @brief Immediately play a sound clip
         * @param clip Clip to play
         * @param length Length of clip (bytes)
         * @return QEAUDIO_RETURN_* code
         * May be called if items in queue
         * Will wait at least for currently executing command or play request
         * If there are any items in queue, will likely (but not assuredly)
         *   be played after currently executing command and before next item
         *   in queue
         */
        int playClip(char clip[], int length);
        /**
         * @brief Immediately play a sound clip
         * @param filename Filename of clip to play
         * @return QEAUDIO_RETURN_* code
         * May be called if items in queue
         * Will wait at least for currently executing command or play request
         * If there are any items in queue, will likely (but not assuredly)
         *   be played after currently executing command and before next item
         *   in queue
         */
        int playClip(char *filename);         

    protected:
	enum CQEAudioControllerCommandType {
            AUDIOCOMMAND_TYPE_TONE = 0,
            AUDIOCOMMAND_TYPE_CLIP,
            AUDIOCOMMAND_TYPE_ENABLE,
            AUDIOCOMMAND_TYPE_DISABLE,
            N_AUDIOCOMMAND_TYPES
        };

	struct CQEAudioControllerCommand
	{
	    CQEAudioControllerCommandType type;
	    unsigned int seqnum;

	    union {
	        struct {
	            long frequency;
	            int amplitude;
	            long duration;
	        } tone;
	        struct {
  	            char * clip;
	            int clipLength;
	        } clip;
	    } data;
	};

        C9302Hardware *m_p9302hw; 
 
	bool enabled;
	void doEnable(void);
	void doDisable(void);

	static void *AudioThread(void *arg);
        pthread_t m_thread;
        bool m_threadRun;
 
        pthread_mutex_t m_mutex;
        pthread_mutex_t m_playmutex;
        pthread_cond_t m_notemptycond;
        pthread_cond_t m_notfullcond;

	static int doPlayTone(C9302Hardware *m_p9302hw, long frequency, int amplitude, long duration);
        static int doPlayClip(char clip[], int length);
        static int doPlayClip(char *filename);

        unsigned int next_seqnum;

	std::queue<CQEAudioControllerCommand> audioCommands;
        unsigned int audioCommands_size;
	std::map<unsigned int, int> err_codes;

	int enqueueCommand(CQEAudioControllerCommand &command, unsigned int &seqnum, unsigned int maxwait);
	void emptyAudioCommandQueue(void);
        int executeAudioCommand(CQEAudioControllerCommand &command);
	static int getCommandSize(CQEAudioControllerCommand &command);

};

#endif

