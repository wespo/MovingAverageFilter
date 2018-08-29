//4 sample moving average filter example

#define DELAY_BUFFER_LEN 4
int delayBuffer[DELAY_BUFFER_LEN] = {0};
long savedSum = 0;
int oldestSampleIndex = 0;
int movingAverageFilter(int currentSample, int* delayBuffer, int delayBufferLen, long& savedSum, int& oldestSampleIndex, bool highPass = false)
//computationally efficient moving average filter algortihm that stores previous results.
//currentSample = new data.
//delayBuffer = buffer to store previous delayBuffer values. Must be delayBufferLen long. Initialize buffer to 0.
//delayBufferLen = number of previous values to average.
//savedSum = pointer to stored previous sum. Used to trade computation time for 4 bytes of memory. Initialize to 0.
//oldestSampleIndex = pointer to oldest sample in the buffer
//highPass = if true, do high pass, otherwise, low pass.
{
   //retrieve oldest sample
   int oldestSample = delayBuffer[oldestSampleIndex];

   //store newest sample and advance pointer to next oldest sample.
   delayBuffer[oldestSampleIndex] = currentSample;
   
   //advance pointer to next oldestSample.
   oldestSampleIndex = (oldestSampleIndex+1)%delayBufferLen; 

   //generate current average by removing oldest sample, and adding newest sample, divide.
   savedSum = savedSum + currentSample - oldestSample;
   int currentAverage = 0;
   if(savedSum != 0) //obvious sanity check
   {
     //int currentAverage = savedSum / delayBufferLen; //simpler fixed point division, but truncates instead of rounds
     currentAverage = div_to_nearest(savedSum, delayBufferLen); //fixed point division with rounding
   }

   if(highPass) //if high pass, subtract current average from summing node.
   {
     currentAverage = currentSample - currentAverage;
   }

   return currentAverage;
}

int div_to_nearest(long n, int d) 
//C++ integer division truncates. This handles that. 
//From: https://stackoverflow.com/questions/8136974/c-functions-for-integer-division-with-well-defined-rounding-strategy
{
  if (n < 0) {
    return (n - d/2 + 1) / d;
  } else {
    return (n + d/2) / d;
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Enter an integer and receive the average of the last four entries.");
}

void loop() {
  while(!Serial.available()) {}
  // put your main code here, to run repeatedly:
  int currentSample = Serial.parseInt();
  Serial.print("Recieved: ");
  Serial.print(currentSample);

  int filterLow = movingAverageFilter(currentSample, delayBuffer, DELAY_BUFFER_LEN, savedSum, oldestSampleIndex);
  
  Serial.print(".\tAverage of last 4 values = ");
  Serial.println(filterLow);
}

