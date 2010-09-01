#ifndef _SONARCONT_H
#define _SONARCONT_H
  
#include "sonar.h"

/**
 * CSonarController is used to automate the firing of multiple sonar modules
 * (sonar arrays).  After setting up a sonar array, this class will 
 * automatically fire the sonar array in the order specified in the background.
 * It will also collect the most recent distance readings of the array.  
 * See CSonar for single sonar methods.
 */ 
class CSonarController
{
public:
  /** 
   * default constructor. 
   */
  CSonarController();
  
  /** 
   *  This constructor takes the necessary arguments to set up a working 
   *  sonar array.
   *  @param numSonar the number of sonars in the sonar array. 
   *  @param sonarArray an array of pointers to CSonar instantiations. 
   *  The order of the CSonar pointers determines the firing order of 
   *  the sonar array.  
   *  @param fireIntervalMs the amount of time in milliseconds to wait 
   *  between subsequent sonar firings.  This value should not be below 
   *  10 ms.
   *  @return 0 if success -1 if error.
   */
  CSonarController(unsigned int numSonar, CSonar *sonarArray[], unsigned int fireIntervalMs);

  ~CSonarController();
  
  /** 
   *  This method takes the necessary arguments to set up a working 
   *  sonar array.
   *  @param numSonar the number of sonars in the sonar array. 
   *  @param sonarArray an array of pointers to CSonar instantiations. 
   *  The order of the CSonar pointers determines the firing order of 
   *  the sonar array.  
   *  @param fireIntervalMs the amount of time in milliseconds to wait 
   *  between subsequent sonar firings.  This value should not be below 
   *  10 ms.
   *  @return 0 if success -1 if error.
   */
  int SetFiring(unsigned int numSonar, CSonar *sonarArray[], unsigned int fireIntervalMs);

  /**
   * Stops the firing of the sonar array. 
   * @return 0 if success -1 if error.
   */
  int EndFiring();

  /**
   * Get the most recent sonar distance values in inches.  
   * @param vals the int array where the sonar distance values will be 
   * copied.  
   * @return 0 if success, -1 if error.
   */
  int GetVals(int vals[]);

private:
  void SetSignal(bool set);
  void SetAlarm(int intervalMs);

  static void SigHandler(int signum);
  
  static unsigned int m_currentIndex;
  static unsigned int m_numSonar;
  static CSonar *m_sonarArray[QEG_NUM_IO];
};

#endif
