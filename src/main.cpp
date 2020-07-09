#include <iostream>

using namespace std;

#ifdef __cplusplus
extern "C"
{
#endif

    void app_main(void)
    {
        while(1)
        {
            cout << "Hello world" << endl;
        }
    }

#ifdef __cplusplus
}
#endif