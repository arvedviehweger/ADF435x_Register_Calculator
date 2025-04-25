A simple C program to calculate the registers of the ADF4350/ADF4351 series of chips.

# Overview:

```
int main() {

    int inputFreq = 2000;

    printf("Please enter a target frequency in MHz: \n");
    scanf("%d", &inputFreq);

    calculateRegister(inputFreq);

    printf("\n");
    print_binary(REGISTER_0);
    printf("\n");
    print_binary(REGISTER_1);
    printf("\n");
    print_binary(REGISTER_2);
    printf("\n");
    print_binary(REGISTER_3);
    printf("\n");
    print_binary(REGISTER_4);
    printf("\n");
    print_binary(REGISTER_5);
    printf("\n");
    

    return 0;

}
```

The values for the output power, reference frequency, auxilary output etc are all hard coded in the `calculateRegister()` function.
They can be changed with the help of the datasheet.

The code may be adapted for use on microcontrollers. Tested on STM32 and AVR platforms.