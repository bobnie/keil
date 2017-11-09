SET PATH=F:\nc\ARM\ARMCC\bin\;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Program Files (x86)\Microsoft SQL Server\100\Tools\Binn\;C:\Program Files\Microsoft SQL Server\100\Tools\Binn\;C:\Program Files\Microsoft SQL Server\100\DTS\Binn\;C:\Program Files (x86)\Windows Kits\8.1\Windows Performance Toolkit\;C:\Program Files\Microsoft SQL Server\110\Tools\Binn\;C:\Program Files (x86)\Microsoft SDKs\TypeScript\1.0\;C:\Program Files\Microsoft SQL Server\120\Tools\Binn\;C:\Users\Admin\AppData\Local\Microsoft\WindowsApps;
SET CPU_TYPE=STM32F103ZE
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=MyFirstjob
SET CPU_CLOCK=0x007A1200
"F:\nc\ARM\ARMCC\bin\ArmAsm" --Via ".\obj\cortexm3_macro._ia"
"F:\nc\ARM\ARMCC\bin\ArmAsm" --Via ".\obj\stm32f10x_vector._ia"
"F:\nc\ARM\ARMCC\bin\ArmCC" --Via ".\obj\stm32f10x_flash.__i"
"F:\nc\ARM\ARMCC\bin\ArmCC" --Via ".\obj\stm32f10x_gpio.__i"
"F:\nc\ARM\ARMCC\bin\ArmCC" --Via ".\obj\stm32f10x_lib.__i"
"F:\nc\ARM\ARMCC\bin\ArmCC" --Via ".\obj\stm32f10x_rcc.__i"
"F:\nc\ARM\ARMCC\bin\ArmCC" --Via ".\obj\stm32f10x_usart.__i"
"F:\nc\ARM\ARMCC\bin\ArmCC" --Via ".\obj\stm32f10x_it.__i"
"F:\nc\ARM\ARMCC\bin\ArmCC" --Via ".\obj\main.__i"
"F:\nc\ARM\ARMCC\bin\ArmLink" --Via ".\obj\MyFirstjob.lnp"
F:\nc\ARM\ARMCC\bin\fromelf.exe ".\obj\MyFirstjob.axf" --i32combined --output ".\obj\MyFirstjob.hex"
