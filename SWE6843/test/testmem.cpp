#include <iostream>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/unistd.h>
#include <bitset>
#include <time.h>

#define GPIO_BASE 0x3F200000
#define BLOCK_SIZE      (4*1024)

#define IN_GPIO(g)      *(address + ((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g)      *(address + ((g)/10)) |= (1<<(((g)%10)*3))

#define GPIO_SET  *(address + 7)
#define GPIO_CLR  *(address + 10)

#define GPIO_READ(g)    *(address + 13) &= (1<<(g))

#define GPFSEL    (address)
#define GPSET     (address + 7)
#define GPCLR     (address + 10)
#define GPLEV     (address + 13)
#define GPPUD     (address + 37)
#define GPPUDCLK  (address + 38)

#define TEST_PIN0  16
#define TEST_PIN1  20
#define TEST_PIN2  4
#define TEST_PIN3  5

void nanowait(long nanoseconds)
{
      timespec ts;
      ts.tv_sec = 0;
      ts.tv_nsec = nanoseconds;
      
      nanosleep(&ts, &ts);
}

int main()
{
      // open handle to memory
      int memoryHandle;
      if ((memoryHandle = open("/dev/mem", O_RDWR|O_SYNC)) < 0)
      {
            std::cout << "Failed to get memory handle.\n";
            return -1;
      }
      
      unsigned long memAddress = GPIO_BASE;
      void* map;
      map = mmap(NULL, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memoryHandle, memAddress);
      
      if (map == MAP_FAILED)
      {
            std::cout << "Failed to map memory.\n";
            return -1;
      }
      
      volatile unsigned int* address = (volatile unsigned int*)map;

      char binaryStr[33];
       
      unsigned int setInput = 7 << ((TEST_PIN0%10)*3);
      unsigned int setOutput = 1 << ((TEST_PIN0%10)*3);
      
      std::cout << "GPFSEL:      " << std::bitset<32>(*GPFSEL) << "\n";
      std::cout << "GPSET:       " << std::bitset<32>(*GPSET) << "\n";
      std::cout << "GPCLR:       " << std::bitset<32>(*GPCLR) << "\n";
      std::cout << "GPLEV:       " << std::bitset<32>(*GPLEV) << "\n";
      std::cout << "GPPUD:       " << std::bitset<32>(*GPPUD) << "\n";
      
      std::cout << "input bit: " << std::bitset<32>(setInput) << "\n";
      std::cout << "out bit:   " << std::bitset<32>(setOutput) << "\n";
      
      *(GPFSEL + (TEST_PIN0/10)) &= ~setInput;
      std::cout << "set in:    " << std::bitset<32>(*(GPFSEL + (TEST_PIN0/10))) << "\n";
      *(GPFSEL + (TEST_PIN0/10)) |= setOutput;
      std::cout << "set out:   " << std::bitset<32>(*(GPFSEL + (TEST_PIN0/10))) << "\n";
      
      unsigned int setInputPin1 = (7<<((TEST_PIN1%10)*3));
      unsigned int setOutPin1 = (1<<((TEST_PIN1%10)*3));
      std::cout << "input bit: " << std::bitset<32>(setInputPin1) << "\n";
      std::cout << "out bit:   " << std::bitset<32>(setOutPin1) << "\n";
      
            *(GPFSEL + (TEST_PIN1/10)) &= ~(setInputPin1);
      std::cout << "set in:    " << std::bitset<32>(*(GPFSEL + (TEST_PIN1/10))) << "\n";
      *(GPFSEL + (TEST_PIN1/10)) |= (setOutPin1);
      std::cout << "set out:   " << std::bitset<32>(*(GPFSEL + (TEST_PIN1/10))) << "\n";
      
            *(GPFSEL + (TEST_PIN2/10)) &= ~(7<<((TEST_PIN2%10)*3));
      std::cout << "set in:    " << std::bitset<32>(*(GPFSEL + (TEST_PIN2/10))) << "\n";
      *(GPFSEL + (TEST_PIN2/10)) |= (1<<((TEST_PIN2%10)*3));
      std::cout << "set out:   " << std::bitset<32>(*(GPFSEL + (TEST_PIN2/10))) << "\n";
      
            *(GPFSEL + (TEST_PIN3/10)) &= ~(7<<((TEST_PIN3%10)*3));
      std::cout << "set in:    " << std::bitset<32>(*(GPFSEL + (TEST_PIN3/10))) << "\n";
      *(GPFSEL + (TEST_PIN3/10)) |= (1<<((TEST_PIN3%10)*3));
      std::cout << "set out:   " << std::bitset<32>(*(GPFSEL + (TEST_PIN3/10))) << "\n";

      //IN_GPIO(4);
      //OUT_GPIO(4);

      *GPCLR = 0xFFFFFFFF;
      nanowait(500000);
      *GPSET = 0xFFFFFFFF;
      nanowait(500000);
      *GPCLR = 0xFFFFFFFF;
      nanowait(500000);
      /*
      *GPCLR |= 1 << TEST_PIN0;
      *GPCLR |= 1 << TEST_PIN1;
      *GPCLR |= 1 << TEST_PIN2;
      *GPCLR |= 1 << TEST_PIN3;
      */
      int bitToSet = 0;
      
      int v = 0;      
      for (int i = 0; i < 10; i++)
      {
            std::cout << "Pin value before " << i << " = " << std::bitset<32>(*GPLEV) << "\n";
            
            if (i & 0x01)
            {
                        *GPSET       |= (1 << TEST_PIN0);
                        nanowait(500);
            }
            else
            {
                  *GPCLR |= (1 << TEST_PIN0);
                  nanowait(500);
            }
            
            if (i & 0x02)
            {
                        *GPSET       |= (1 << TEST_PIN1);
                        nanowait(500);
            }
            else
            {
                  *GPCLR |= (1 << TEST_PIN1);
                  nanowait(500);
            }
            
            if (i & 0x04)
            {
                        *GPSET       |= (1 << TEST_PIN2);
                        nanowait(500);
            }
            else
            {
                  *GPCLR |= (1 << TEST_PIN2);
                  nanowait(500);
            }
            
            if (i & 0x08)
            {
                        *GPSET       |= (1 << TEST_PIN3);
                        nanowait(500);
            }
            else
            {
                  *GPCLR |= (1 << TEST_PIN3);
                  nanowait(500);
            }            
            sleep(1);
            
            std::cout << "Pin value after " << i << "  = " << std::bitset<32>(*GPLEV) << "\n";
            
            
            //*GPCLR |= 1 << TEST_PIN;
            sleep(1);
            
      }

      //clear everything
      *GPFSEL &= 0;

      *GPCLR = 0xFFFFFFFF;
      /*
      *GPCLR |= 1 << TEST_PIN0;
      *GPCLR |= 1 << TEST_PIN1;
      *GPCLR |= 1 << TEST_PIN2;
      *GPCLR |= 1 << TEST_PIN3;
      */
      
      nanowait(500000);
         
      std::cout << "GPFSEL:      " << std::bitset<32>(*GPFSEL) << "\n";
      std::cout << "GPSET:       " << std::bitset<32>(*GPSET) << "\n";
      std::cout << "GPCLR:       " << std::bitset<32>(*GPCLR) << "\n";
      std::cout << "GPLEV:       " << std::bitset<32>(*GPLEV) << "\n";
      std::cout << "GPPUD:       " << std::bitset<32>(*GPPUD) << "\n";
 
      munmap(map, BLOCK_SIZE);
      close(memoryHandle);
      
      return 0;
}
