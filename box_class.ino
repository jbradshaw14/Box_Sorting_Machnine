/* This class defines the box object. Each box has a specific name, number
   and range of color values that is seen by the color sensor
   Created by John B. Bradshaw 2019
*/
class Box{
    private:
        int boxNum; 
        char boxName;
        int colorRangelow;
        int colorRangehigh;
    public:
    // ~constructor - assign a box number to each box object 0 - 5
    Box(int bn) //bn = 0-5
    {
        boxNum = bn;
        switch(boxNum)
        {
            case 0 : boxName = 'no box ';
            case 1 : boxName = 'skittles ';
            case 2 : boxName = 'coffee ' ; 
            case 3 : boxName = 'popcorn ';
            case 4 : boxName = 'clear box ';
            case 5 : boxName = 'clear short box ';
            default : boxName = 'Invalid Input! '; 
        }
    }

    // Set functions
    void setRangeClear(int lr, int hr) // set clear lower and upper bounds
    { colorRangelow = lr; colorRangehigh = hr;  }
   
   // Get functions
    int getColorRangelow()
    { return colorRangelow; } 
    int getColorRangehigh() 
    { return colorRangehigh; } 
    int getBoxNum() 
    {   return boxNum;  } 

    char getBoxName()
    {  return boxName;  } 
};