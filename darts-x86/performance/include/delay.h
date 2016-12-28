void delayLoop(int delaylength)
{

    int  i; 
    float a=0.; 

    for (i=0; i<delaylength; i++) a+=i; 
    if (a < 0) std::cout << a; 

} 