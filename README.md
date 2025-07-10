这是我们团队参加2025年嵌入式大赛的作品：基于CH32V307的毫米波雷达高精度探测和盲人智能导航系统
在MounRiver Studio开发平台运行，使用的是沁恒最小系统开发板CH32V307VCT6
摘要
本系统基于RISC-V主控芯片CH32V307VCT6，本项目采用模块化设计、多传感器协同与低功耗架构，集成毫米波雷达、陀螺仪、4G通信模块和离线语音模块，实现导盲与安全防护。核心功能包括：
1. 高精度环境感知​：毫米波雷达实时探测一定范围内静、动态目标，可实现障碍物检测和物体靠近警报。
2. 跌倒检测与求救​：结合陀螺仪加速度/角速度数据，可迅速识别跌倒行为，并及时发送求救短信至预设联系人。
3. 无障碍交互​：通过语音模块实现快速精确的交互；可使用蓝牙音频输出保障隐私。
第一部分 作品概述
1.1 功能特性
本系统主控板采用CH32V307VCT6，核心功能为基于毫米波雷达实现0-8米范围内的高灵敏度实时探测，可精准感知相对运动物体（如障碍物、行人、自行车等），支持物体靠近时主动警报、语音指令被动探测两种检测功能。系统集成了陀螺仪来实现跌倒识别功能，当检测到摔倒时将触发延时求救机制，若未及时取消则通过移动通信模块air780e向预设应急联系人发送求救短信。为提升人机交互体验，系统采用语音交互方式，特别优化了对盲人用户的操作友好性，同时扩展蓝牙模块，可连接蓝牙耳机，兼顾使用隐私性与操作便利性。
1.2 应用领域
a）盲人出行辅助：作为盲人日常出行的重要工具，帮助其安全地在城市街道、商场、公园等各种复杂环境中行走，有效规避障碍物，减少出行风险。
b）特殊人群关怀：除盲人外，也可应用于行动不便的老年人或其他有类似需求的特殊人群，为他们提供安全保障与便捷的辅助功能。
c）智能穿戴设备拓展：该系统的技术与功能可作为智能穿戴设备的重要组成部分，推动智能穿戴设备在健康监测与安全辅助领域的发展。
1.3 主要技术特点
a）高精度探测：采用毫米波雷达技术，具备高分辨率、强抗干扰能力，可在复杂环境下实时、精准地感知周围动态物体的距离、速度和方向信息，探测精度满足盲人步行避障需求。
b）多传感器融合：将毫米波雷达与陀螺仪相结合，实现环境感知与人体状态监测的功能互补，提升系统对用户安全状况判断的准确性。
c）智能应急响应：基于 air780e 模块的应急报警功能，检测到用户跌倒等紧急情况时，能使用4G移动通信协议发送求救短信，及时通知紧急联系人。
d）自然语音交互：支持语音指令输入与语音反馈，采用语音合成与识别技术，实现与盲人用户的自然交互，还提供了蓝牙功能接口，方便用户根据需求连接蓝牙耳机，操作简单易懂，无需复杂学习过程。
e）功能可扩展性：采用模块化编程，各个外设间耦合性弱，为后续系统功能升级与扩展提供可能，如增加地图导航、社交互动等功能。
1.4 主要性能指标
主要性能指标

指标项目
具体参数
毫米波雷达探测距离
法向0 - 8 米
毫米波雷达探测精度
0.1m
毫米波雷达探测角度
方位角±60° ， 俯仰角±30°  
毫米波雷达检测目标数上限
3
跌倒识别加速度阈值
低于0g（重力加速度）或高于2.1g
跌倒识别角速度阈值
22.9°/s
跌倒识别短时姿态角阈值
20° 
跌倒警报取消时间
10s
跌倒识别时间
0.5秒
物体靠近警报速度采样时间
0.05s/次
物体靠近警报速度阈值
-1.1m/s
物体靠近警报持续时间阈值
0.15s
物体靠近警报响应时间
2 秒内
语音指令识别准确率
不低于 95%
语音指令响应速度
0.1秒
正常工作模式功耗
雷达460mW+语音模块200mW+OLED显示屏20mW+陀螺仪18mW+CH32核心板150mW+蓝牙模块130mW+移动通信模块40mW = 1.02W
续航时间
20h以上
1.5 主要创新点
a）多模态感知融合技术：毫米波雷达与陀螺仪的协同应用，形成“外部环境+用户姿态”的双重安全监测体系。
b）面向盲人用户的自然交互设计：无障碍语音交互，支持盲人通过语言沟通系统。蓝牙扩展保障使用私密性和无噪声性（避免语音外放）。
c）智能应急响应机制：延时求救与主动取消，避免误报；若未取消则发送求救短信，平衡了安全性与误触风险。Air780e实现4G通信，成本更低且信号范围更广。
d）模块化与可扩展架构：硬件与软件解耦，主控与外设通过标准接口连接，软件模块化编程，便于快速迭代。
1.6 设计流程
a）思路创新与毫米波雷达
我们自主选题的原因就是想做别人没有做过的事情，盲人辅助设备存在市场空白，毫米波雷达前景火热，二者搭配是我们给出的答案。
b）系统集成构思与可行性
一个毫米波雷达不足以搭建一个完善的系统，深思熟虑后我们还集成了语音模块、陀螺仪、4G通信和蓝牙模块，可构成一个功能齐全的盲人辅助设备。
c）工程实现与硬件连接
MRS搭建软件模块化编程，主控芯片通过USART、I2C等接口连接各模块，合理安排诸多功能，分配硬件接口，有序的组织起一个完整的工程结构。
d）测试验证与未来展望
通过不断的debug和优化，实现了所有预期功能，但诸多地方需要优化，也预留了未来添加其他外设和组件的接口，希望能有更多的扩展方案。
第二部分  系统组成与功能说明
2.1 整体介绍
[图片]
https://bcnvav0pnhbv.feishu.cn/sync/DYiadcYbJsDJTxbxaRgcmAHZntb
系统整体架构如图 2-1 所示，主要由主控板CH32V307、毫米波雷达模块、陀螺仪模块、air780e 通信模块、语音模块、电源模块以及蓝牙音频发射模块组成。主控板 CH32V307 作为系统核心，负责协调各模块工作，处理传感器数据，执行单多目标避障提醒、跌倒检测算法等，并控制各功能模块实现相应操作。
毫米波雷达模块实时探测周围环境，获取动态物体信息；陀螺仪模块监测用户姿态变化，用于跌倒检测；air780e 通信模块在紧急情况下发送求救信息；语音交互模块实现语音指令接收与反馈；电源模块为系统各部分提供稳定电力；蓝牙音频发射模块可连接蓝牙耳机，实现语音信息的私密播放。
系统工作流程为：开机后，各模块初始化，毫米波雷达与陀螺仪开始实时采集数据并传输给主控板。主控板对数据进行处理分析，若检测到障碍物，根据算法判断物体方位和速度，通过语音交互模块提示运动物体方向；若检测到用户跌倒，主控板和语音模块协作进行延时判断，如果没有收到取消指令立即控制 air780e 模块发送求救信息。用户可通过语音指令与系统交互，切换雷达检测模式或进行其他操作，系统将处理结果以语音形式反馈给用户。
暂时无法在飞书文档外展示此内容
https://bcnvav0pnhbv.feishu.cn/sync/UEBhd9B08s0ds2bqrPjcqGZLnlf
2.2 硬件系统介绍
2.2.1 硬件整体介绍
整个硬件系统由主控板CH32V307VCT6、毫米波雷达模块Rd-03D_V2、通信模块air780e、语音模块ASRPRO2.0、电源模块、陀螺仪模块MPU6050、蓝牙音频发射模块KCX_BT_EMITTER组成。
2.2.2 主控板 CH32V307VCT6
CH32V307 是基于青稞 32 位 RISC-V4F 内核，多种指令集组合的高性能微控制器，具备丰富的外设资源与强大的处理能力。其集成了高速 USB、CAN、SPI、I2C、USART 等多种通信接口，可方便地与各功能模块进行数据交互。工作频率最高可达 144MHz（本系统采用96MHz），内置256K Flash 与64K SRAM，能够满足系统程序存储与数据处理需求。在本系统中，CH32V307 作为核心控制单元，运行障碍物检测算法、跌倒检测算法，处理语音模块交互指令及数据，控制通信模块等，实现系统整体功能。
[图片]
图 2-3 CH32V307引脚信息图
[图片]
https://bcnvav0pnhbv.feishu.cn/sync/Q8WAduBRvsRvAPb2cSTcsGoln6c
CH32V307采用多总线架构，高速外设总线（APB2）和低速外设总线（APB1）分工明确，驱动不同外设，以优化系统性能和功耗。高速外设总线连接高带宽需求的模块（如ETH、USB、FSMC），运行在高时钟频率（接近或与CPU主频相同），支持DMA和突发传输，确保数据吞吐效率。低速外设总线则服务于低实时性外设（如I2C、UART、基本定时器），通过降频或独立开关降低功耗，避免占用高速总线资源。这种分层设计使CH32V307能同时满足高性能和低功耗需求。
[图片]
图 2-5 CH32V307最小系统原理图
CH32V307VCT6最小系统板集成了多种外设功能，包括ETH（以太网接口）、三个POWER&USB接口（支持USB通信和供电），以及板载WCH_LinkE调试器（可用于代码烧录和串口调试）。此外，该开发板还提供ARDUINO兼容接口、用户按键、LED指示灯和必要的保护电路。
2.2.3 毫米波雷达模块 Rd-03D_V2
选用毫米波雷达传感器Rd-03D_V2，该模块通过USART与主控板连接，以256000bps的串口速率实时向主控板传输探测到的周围物体坐标、速度信息。
24GHz毫米波雷达天线利用高频电磁波的特性实现目标探测。其核心原理是通过发射24GHz频段的毫米波（波长约12.5mm），利用天线阵列接收目标反射的回波信号，再通过信号处理计算目标的距离、速度和方位。短波长特性使雷达具备高分辨率，可检测微小目标和低速运动，同时天线尺寸能做得更紧凑，适合集成到小型设备中。
天线通常采用微带贴片或波导缝隙设计，通过多单元组成阵列来提升方向性和增益。波束形成技术（如相控阵）可电子控制波束方向，无需机械转动。雷达多采用FMCW（调频连续波）工作模式，通过分析发射与接收信号的频率差来精确测距和测速。
[图片]
图 2-6 ICL1122 芯片架构图  
该模组硬件由 AIoT 毫米波雷达芯片 ICL1122、 高性能一发两收微带天线和 MCU 及外围辅助电路组成。智能算法固件采用 FMCW 波形和 ICL1122芯片专有的先进信号处理技术。  
[图片]
图 2-7 RD-03D_V2主要参数说明
[图片]
图 2-8 雷达跟踪范围示意图
2.2.4 通信模块 air780e
air780e 是一款 4G Cat.1 通信模块，实现最高下行速率达 10Mbps、上行速率达 5Mbps 的高速数据传输，支持可靠的短信收发功能。该模块提供 USB 和 UART 两种接口，可根据主控板的需求灵活选择。
在设备运行过程中，一旦主控板监测到延时跌倒判定，便会迅速向 air780e 模块发送指令。接收到指令后，air780e 模块能够访问内部存储的预设联系人信息，根据主控板发送的串口数据生成告警短信，并通过天线发送，确保应急联系人第一时间获取异常信息。
在本硬件连接设计电路中使用UART5接口进行串口通信：PC12（TX）和 PD2（RX）与微处理器连接。MCU通过发送AT指令来实现特定的功能，如对指定的电话号码发送任意文本的短信等。
其该模块部分原理图如下：
[图片]
https://bcnvav0pnhbv.feishu.cn/sync/CpHod9XSYsC4sWbgDBocQpURn1d
其中V_IN为3.3~4.2V供电接口，GND为接地端，TXD/RXD为发送端和接收端，DTR为低功耗控制管脚，3.3-5V高电平唤醒，EN为使能端口，VBUS为USB总线电压。
2.2.5 语音模块 ASRPRO 2.0
ASRPRO 2.0是一款用于离线语音应用设计的低成本平台，搭载高性能语音处理芯片和神经网络处理器(NPU)，支持DNN/TDNN/RNN等多种复杂模型架构。开发板自带麦克风与状态指示灯，提供丰富的接口资源，并搭载先进的单麦克风降噪技术，具备回声消除和环境噪声抑制能力，实现360°全方位精准拾音，在嘈杂环境中仍能保持高识别率。该开发板支持中英日等多语种离线语音识别及声纹识别功能。
该模块在项目中用于实现硬件和使用者的交互。一方面用户能够使用特定语音指令控制检测模式的切换、使用静止物体探测功能，另一方面会向用户播报交互反馈、检测结果、跌倒预警信号、人/物靠近信号等，为用户提供操作便利性。
在本项目中语音模块通过自身的UART0(TX-PB5、RX-PB6)、UART1(TX-PA2、RX-PA3)分别和核心板的USART2（TX-PA2、RX-PA3)、UART4(TX-PC10、RX-PC11)连接来实现串口通信。
以下是语音模块原理图：
[图片]
https://bcnvav0pnhbv.feishu.cn/sync/VFHcdUxnLsf3ivbS7ficDIdBnYf
图中AEC为回声消除电路，MIC和SPK分别为麦克风输入和音频输出电路，MCU为语音模块的核心板。
2.2.6 陀螺仪模块MPU6050
采用三轴陀螺仪传感器，通过 I2C 接口与主控板相连，实时监测用户在三个维度上的姿态变化。陀螺仪模块具备高精度、低功耗特点，可精确检测人体的倾斜、旋转等动作。主控板根据陀螺仪采集的数据，结合预设的跌倒检测算法，判断用户是否发生跌倒。一旦检测到跌倒事件，立即触发应急报警流程。
MPU-6050 为6 轴运动处理传感，集成 3 轴 MEMS 陀螺仪（测量角速度）和 3 轴 MEMS 加速度计（测量加速度），通过 16 位 ADC 将模拟量转换为数字量输出。其关键参数如下：
a）陀螺仪测量范围：±250/±500/±1000/±2000°/ 秒（dps），默认精度 131 LSBs/°/sec；
b）加速度计测量范围：±2/±4/±8/±16g，可通过寄存器配置量程；
c）通信接口：仅支持 I2C（100kHz），通过 VLOGIC 引脚适配 1.8V~VDD 的逻辑电平。
硬件连接设计电路中使用 PB6（SCL）和 PB7（SDA）作为 I2C 通信引脚，通过 I2C 总线与微处理器连接。AD0接地保证其I2C地址为0x68，硬件上需外接 3 个关键电容：
a）0.1uF 校准滤波电容（REGOUT 引脚）；
b）0.1uF VDD 旁路电容（VDD 引脚）；
c）10uF 电荷泵电容（CPOUT 引脚），确保传感器稳定工作。
其原理图如下：
[图片]

https://bcnvav0pnhbv.feishu.cn/sync/BUcsdpnLosvg2RbZkIgc2mNKnJe
2.2.7 蓝牙音频发射模块 KCX_BT_EMITTER
蓝牙模块接口可实现与蓝牙耳机的连接。当用户连接蓝牙耳机后，系统的语音反馈信息将通过蓝牙传输至耳机播放，避免语音外放带来的隐私泄露问题，同时也为用户在嘈杂环境中使用系统提供便利。
KCX_BT_EMITTER的CAD尺寸图如下：
暂时无法在飞书文档外展示此内容
https://bcnvav0pnhbv.feishu.cn/sync/QuSqdAfR1sLYKQbOLTrcawijnnd
接线原理图如下：
[图片]
https://bcnvav0pnhbv.feishu.cn/sync/VZ07dXPJ3sk1iMbsbzgcpMksnse
蓝牙音频模块工作于发射模式，+5V接电源正极，PGND为电源地，LED为状态指示灯，RX/TX为串口接收和发送脚，IN_L/R为音频的左右声道输出，AGND为音频地。
2.3 软件部分
2.3.1 主控板项目结构
[图片]
图 2-14 项目结构
a）二进制  
二进制文件夹存放的是项目编译后生成的最终可执行文件（.elf），该文件包含机器代码、调试信息和内存布局，可直接用于烧录到芯片或调试程序。
b）Includes  
该文件夹存放项目所需的头文件（.h），包括芯片厂商提供的库文件、第三方组件声明以及项目自定义的公共定义。头文件通常包含函数声明、宏定义和数据结构，供其他源文件引用，确保代码模块化和可复用性。  
c）Core  
Core 文件夹包含与微控制器核心相关的底层代码，例如芯片寄存器定义、系统时钟配置、中断控制器（NVIC）初始化等。这些代码直接与硬件相关，确保芯片的基础功能（如时钟、复位、中断）能够正确运行，为上层应用提供稳定的硬件支持。  
d）Debug  
Debug 文件夹存放与调试相关的配置和工具，例如调试脚本（如 GDB/OpenOCD 配置）、日志输出模块或特殊调试功能的实现代码。这些文件帮助开发者在开发过程中定位问题，优化性能，并确保系统在运行时符合预期。 
e）Ld
Ld（Linker）文件夹包含链接器脚本（.ld 文件），用于定义程序在内存中的布局，包括 Flash（存储代码）和 RAM（存储运行时数据）的分配。链接脚本直接影响程序的运行方式，例如代码段、数据段、堆栈的起始地址和大小，确保程序能正确加载和执行。  
f）Peripheral  
Peripheral 文件夹存放芯片外设的驱动代码，例如 GPIO、UART、SPI、I2C、定时器等。这些驱动通常基于厂商提供的库（如 HAL 或 LL 库）进行封装，提供更易用的接口，方便上层应用调用硬件功能。  
g）Startup  
Startup 文件夹包含系统启动相关的代码，通常是汇编文件（如 startup_ch32v30x.s），负责初始化栈指针、设置中断向量表，并在跳转到主程序（main()）前完成必要的硬件配置。这些代码是程序运行的起点，确保芯片能正确进入应用逻辑。  
h）General_file  
General_file 文件夹是自定义文件夹，分为两个文件夹和主文件夹。主文件用来存放系统级代码，如system.c，init.c，interrupt.c等，保留在主函数或者后台服务中调用这些文件的接口，便于工程项目模块化，增强可移植性；app文件夹存放与具体业务逻辑相关的代码，实现产品的核心功能，但本项目不涉及到HAL层或者硬件驱动层之上；driver文件夹存放硬件外设的底层驱动代码，直接操作寄存器或硬件抽象层（HAL），本项目中有雷达的数据处理和接收，语音模块交互功能的实现，陀螺仪传感器接收数据的处理以及分析，移动通信模块发短信功能的实现。
i）obj  
obj 文件夹是编译过程中生成的中间文件，例如目标文件（.o）、依赖文件（.d）和临时文件。这些文件在最终链接阶段被合并成可执行文件，通常不需要手动修改，但可用于调试编译问题。  
j）User  
User 文件夹存放开发者自定义的代码，例如实验性功能、测试用例或项目特定的配置。这个目录的内容可能因不同开发者或需求而变化，适合存放尚未集成到主干的代码。  
k）246_rander_navi.launch  
该文件是集成开发环境（如 VSCode、Eclipse）的启动配置文件，定义如何烧录程序、启动调试会话以及设置运行参数。例如，它可能指定使用 J-Link 或 OpenOCD （实际为WCH_LinkE）进行调试，并设置 Flash 下载地址。  
2.3.2 主控板核心代码
a）system.c
项目采用 system_init() + system_loop() 的分层架构。
首先通过 system_init() 集中完成所有初始化工作，包括硬件外设的配置（如 GPIO、UART、ADC 等初始化）、驱动模块的加载（mpu模块初始化，雷达信息接收初始化）、系统时钟的设置、全局数据结构的建立等关键启动流程，确保系统进入主循环前处于稳定可靠的初始状态。 system_loop() 在主函数的 while(1) 中以循环方式实现系统核心功能，持续处理传感器数据（雷达，陀螺仪）、通信协议处理（和语音板进行数据指令交互）等实时任务。
这种架构的优势在于：代码结构层次分明，便于团队协作开发；功能模块解耦，提高代码复用性和可维护性；初始化与运行逻辑分离，降低调试复杂度；更容易适应不同硬件平台或操作系统的移植需求；同时也为后续功能扩展保留了良好的灵活性。
关键代码如下，在 system.c 中基本上是调用其他函数，不再展示多余部分：
void USARTx_SendByte(USART_TypeDef *USARTx, uint8_t data)；// 向指定USART发送单个字节
void USARTx_SendString(USART_TypeDef *USARTx, char *str)；// 向指定USART发送字符串（包含终止符'\0'前的所有字符）

void System_Init(void);//在这里进行系统的初始化
void System_Run(void);//在这里写非循环非初始化内容
void System_Loop(void);//在这里写主函数while(1)中实现的功能
b）init.c
在 system.c 中调用了许多初始化函数如：
I2C_Init_Single(I2C1, 100000, GPIOB, GPIO_Pin_6, GPIOB, GPIO_Pin_7);
这是我们自行编写的函数，存放于 init.c 中，类似于STM32HAL库中的简易初始化，将多句结构体赋值初始化集成到一个函数中，便于复杂系统调用多个外设接口。
函数声明及定义如下，其余函数类似于USART的初始化，不再展示:
void RCC_Configuration(void)//在这个函数中初始化所有会用到的总线时钟
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                          RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD |
                          RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB1Periph_USART3 |
                            RCC_APB1Periph_UART4 |RCC_APB1Periph_UART5 |
                          RCC_APB1Periph_I2C1 |RCC_APB1Periph_I2C2 | RCC_APB1Periph_TIM2, ENABLE);
}

void USART_Init_Single(USART_TypeDef* USARTx, uint32_t BaudRate,
                      GPIO_TypeDef* GPIO_TxPort, uint16_t GPIO_TxPin,
                      GPIO_TypeDef* GPIO_RxPort, uint16_t GPIO_RxPin)//这个函数用来初始化单个USART模块，注意需要串口和引脚匹配
{
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    // 配置TX引脚为复用推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_TxPin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIO_TxPort, &GPIO_InitStructure);

    // 配置RX引脚为浮空输入
    GPIO_InitStructure.GPIO_Pin = GPIO_RxPin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIO_RxPort, &GPIO_InitStructure);

    // USART参数配置
    USART_InitStructure.USART_BaudRate = BaudRate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USARTx, &USART_InitStructure);
    USART_Cmd(USARTx, ENABLE);
}

void GPIO_Init_Single(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode, GPIOSpeed_TypeDef GPIO_Speed);//这个函数用来初始化单个GPIO引脚
void I2C_Init_Single(I2C_TypeDef* I2Cx, uint32_t ClockSpeed,
                    GPIO_TypeDef* GPIO_SCLPort, uint16_t GPIO_SCLPin,
                    GPIO_TypeDef* GPIO_SDAPort, uint16_t GPIO_SDAPin);//这个函数用来初始化单个I2C模块
void TIM_Base_Init(TIM_TypeDef* TIMx, uint16_t psc, uint32_t arr);// 定时器基本定时功能初始化（TIM1 - TIM10）
对比STM32 HAL库的GPIO初始化，HAL库的代码的去耦合性更好，总线时钟初始化也集成在外设初始化函数里：
void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();

  /*Configure GPIO pin : PE0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}
c）interrupt.c
interrupt.c用于存放中断服务函数，CH32中，中断服务函数在工程中已经被弱定义，用户使用中断功能时需要自行重写中断服务函数，当有中断时，程序自动跳转至服务函数中，并占用CPU一小段时间。
    .weak   USART1_IRQHandler          /* USART1 */
    .weak   USART2_IRQHandler          /* USART2 */
部分代码如下：
volatile uint8_t rec_count = 0;//用于雷达串口接收的数据指针
volatile uint8_t usart2_cmd_received = 0;//该变量可取6个值，用于读取语音模块发送的数据请求，根据该变量取值来返回不同数据

void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//检测到中断标志
    {
        uint8_t data = USART_ReceiveData(USART1);
        if (rec_count == 0)// 状态1：等待帧头
        {
            if (data == 0xAA)//检测到帧头才开始接收
            {
                rec_data[rec_count++] = data;
            }
            // 忽略非帧头数据
        }
        else if (rec_count < 30)// 状态2：接收剩余数据
        {
            rec_data[rec_count++] = data;
            if (rec_count == 30)// 接收完成检查
            {
                rec_count = 0;// 重置计数器（无论成功与否）
            }
        }
    }
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
}

volatile uint64_t sys_time = 0;// 100us级系统时间戳

void TIM2_IRQHandler(void)//TIM2的基本定时功能，实现雷达切换命令的轮询以及自定义系统时间戳
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) // 检查更新中断标志
    {
        sys_time ++;

        if (decmode_change_flag == 1)
        {
            change_decmode();//切换检测模式
            decmode_change_flag = 0;

            if(detect_flag == 0){
                SendTargetId(12);//发送信号告知语音板当前为单目标
            } else if(detect_flag == 1){
                SendTargetId(13);//发送信号告知语音板当前为多目标
            }
        }
    }

    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);// 清除中断标志
}

void USART1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));//串口收到雷达数据的中断处理
void USART2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));//功能1，语音模块发送请求，要求返回3个目标的距离角度信息
void UART4_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));//功能2，收到语音模块的指令，向雷达发送改变检测模式的命令；功能4跌倒求救信号的取消
void TIM2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

void Delay_NonBlocking(uint64_t delay_us);//使用自定义系统级时间戳实现的非阻塞延时
uint64_t get_sys_time(void);//保留系统时间戳的外部接口
一共有两种中断，定时器中断和串口中断。串口中断负责和外设模块信息和指令的交互，如通过串口接收雷达发送的原始数据帧（固定30字节长度，包含帧头0xAAFF0300和帧尾0x55CC校验）；定时器模块实现两个功能，一是实现一个自定义时间戳，以100us为一次计数周期，可用来实现非阻塞延时，另外一个是轮询，检测切换命令的标识，如果检测到则实现切换雷达模式的功能。
其中__attribute__((interrupt("WCH-Interrupt-fast")))是 WCH针对其 CH32 系列 MCU 设计的中断函数优化指令，它的核心作用是大幅减少中断响应时间。当这个属性修饰中断函数时，编译器会只保存必要的寄存器（而不是全部），并采用 WCH 芯片专用的快速中断处理机制，从而减少压栈/出栈的开销，使中断响应速度比标准中断快30%~50%。
d）rander.c
rander.c 负责雷达的数据处理和指令交互，雷达数据帧定义如下。
[图片]
图 2-15 雷达上报数据帧格式
[图片]
图 2-16 雷达帧内数据帧格式  
该文件完成雷达数据的解析、转换和模式控制等核心功能（采集放在了串口中断里）。
主控板通过串口接收雷达发送的原始数据帧，从中解析处理出最多3个目标的运动信息，数据经过处理后存入全局帧结构体中，包括平面直角坐标系下的X/Y坐标（单位cm，支持正负值）、运动速度（cm/s）和像素距离值（mm）。再通过CartesianToPolar函数将直角坐标转换为极坐标系下的距离和角度（-60-60度），转化为便于人脑直接理解的角度和距离值，供语音模块等其他模块使用。
系统支持两种检测模式切换（通过detect_flag标志位控制），可发送定义好的12字节串口指令来改变雷达工作模式。数据处理过程进行了算法优化，如使用位操作提取数据符号、乘法代替除法等。
关键代码如下：
uint8_t decmode_change_flag = 0;//切换检测模式中断标志，如果收到切换模式的命令则变化
uint8_t detect_flag = 0;//雷达工作模式标志，0是单目标，1是多目标
float distance[3] = {1.0,1.0,1.0};//存放距离角度的数组，用于给语音模块传输数据
float angle[3] = {1.0,1.0,1.0};
uint8_t send_dis[20] = {0};//float转化为ASCII码存放的数组
uint8_t send_ang[20] = {0};
uint8_t send_single[12] = {0xFD,0xFC,0xFB,0xFA,0x02,0x00,0x80,0x00,0x04,0x03,0x02,0x01};//设置为单目标模式的下行串口信息
uint8_t send_multiple[12] = {0xFD,0xFC,0xFB,0xFA,0x02,0x00,0x90,0x00,0x04,0x03,0x02,0x01};//设置为多目标模式
uint8_t rec_data[30] = {0};//从雷达模块接受的串口数据缓冲区
FrameData Frame = {{0,0,0,0},{{1.0f,1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f,1.0f}},{0,0}};//存放收到并处理过后的雷达信息的结构体

//帧结构定义
#define FRAME_HEADER_SIZE 4//帧头4个字节
#define FRAME_TAIL_SIZE 2  //帧尾2个字节
#define DATA_GROUP_SIZE 8  //每组数据8字节
#define DATA_GROUP_COUNT 3 //3组数据

typedef struct//单组数据结构
{
    float x ;           // x坐标，单位cm
    float y ;           // y坐标，单位cm
    float speed ;       // 速度，单位cm/s
    float resolution ;  // 距离分辨率，单位mm
} SingleData;

typedef struct//完整帧结构
{
    uint8_t header[FRAME_HEADER_SIZE];   // 帧头
    SingleData groups[DATA_GROUP_COUNT]; // 数据组
    uint8_t tail[FRAME_TAIL_SIZE];       // 帧尾
} FrameData;

void CartesianToPolar(float x, float y, float* angle_deg, float* distance);//将xy数据转化为距离和角度值
float xy_pro(const uint8_t data[2]);//处理x，y坐标
float speed_pro(const uint8_t data[2]);//处理速度值
float dist_pro(const uint8_t data[2]);//处理像素距离值
bool parse_frame_data(const uint8_t raw_data[30], FrameData *frame);//解析数据并存入结构体
void change_decmode(void);//改变雷达工作模式
e）mpu.c
关键的函数声明如下：
void fallcheck(void);

uint8_t MyI2C_WriteRegister(uint8_t devAddr, uint8_t regAddr, uint8_t data);
uint8_t MyI2C_ReadRegister(uint8_t devAddr, uint8_t regAddr, uint8_t* data);
void MPU6050_Init(void);
void I2C_RecoverBus(void);
void Delay(volatile uint32_t count);

float calculate_angle(void);//计算姿态角
uint8_t MPU6050_ReadAccelGyro(int16_t *accel, int16_t *gyro);//获取加速度、角速度值
fallcheck(void) ：跌倒检测函数，读取 MPU6050 的加速度和角速度数据，计算它们的模值，根据设定的阈值和计算得到的0.5s后用户姿态角，综合判决是否发生跌倒。如果发生 I2C 错误，会尝试恢复总线并重新初始化 MPU6050。
具体实现如下：
void fallcheck(void)
{
     i2c_status = MPU6050_ReadAccelGyro(accel, gyro);

     if(i2c_status == I2C_ERROR_NONE) {
         acc_mag = sqrtf(accel[0]*accel[0] + accel[1]*accel[1] + accel[2]*accel[2]);
         gyro_mag = sqrtf(gyro[0]*gyro[0] + gyro[1]*gyro[1] + gyro[2]*gyro[2]);
         // 简单摔倒检测
         // 在触发预警后增加姿态检查
         if ((acc_mag < 2000 || acc_mag > 35000) && gyro_mag > 1500) {
             // 短暂延迟后读取姿态
             Delay_Ms(500);  // 阻塞式延时等待落地稳定进行姿态角判定
             float angle = calculate_angle(); // 通过加速度计算倾角（示例函数）

             if (angle > 20.0f) {  // 身体倾斜超过20度视为摔倒
                 fallsend_pre();
             }
         }
     }
     else{
         // I2C总线恢复
         I2C_RecoverBus();
         // 重新初始化I2C
         I2C_Init_Single(I2C1, 100000, GPIOB, GPIO_Pin_6, GPIOB, GPIO_Pin_7);//初始化mpu用的I2C1
         MPU6050_Init();
     }

     fall_send();//时间戳&摔倒标志判定是否发送摔倒警报信号
}
MyI2C_WriteRegister(uint8_t devAddr, uint8_t regAddr, uint8_t data)：自定义的 I2C 写寄存器函数，通过 I2C 总线向指定设备的指定寄存器写入数据，处理了总线忙、启动信号、地址发送、数据发送和停止信号等操作，并处理了超时和应答失败等异常情况。
MyI2C_ReadRegister(uint8_t devAddr, uint8_t regAddr, uint8_t* data)：自定义的 I2C 读寄存器函数，通过 I2C 总线从指定设备的指定寄存器读取数据，处理了总线忙、启动信号、地址发送、重复启动信号、数据接收和停止信号等操作，并处理了超时和应答失败等异常情况。
MPU6050_Init(void)：初始化 MPU6050 传感器，通过向 PWR_MGMT_1 寄存器写入 0x00 来清除睡眠位，使传感器退出睡眠模式。
   I2C_RecoverBus(void)：用于恢复 I2C 总线，处理总线异常情况，如设备卡死等。
  void Delay(volatile uint32_t count)：I2C总线恢复用的空指令延时。
MPU6050_ReadAccelGyro(int16_t *accel, int16_t *gyro)：读取 MPU6050 传感器的加速度和角速度数据，处理了总线忙、启动信号、地址发送等操作，并处理了超时异常情况。
mpu.c 实现了完整的 I2C 通信流程控制包括标准协议实现，严格遵循 I2C 协议规范，包括起始条件、设备寻址、寄存器选择、重复起始和数据接收以及多字节读取优化，通过单个 I2C 事务连续读取 14 个字节的数据（6 字节加速度 + 6 字节陀螺仪 + 2 字节温度）。还有健壮的错误处理机制，如：每个关键步骤都设置了超时计数器，防止因通信异常导致程序卡死；明确区分不同类型的错误（超时、ACK 失败），便于上层应用处理；在发生错误时能主动发送停止条件，释放 I2C 总线；还包括精准的 ACK/NACK 控制和动态 ACK 管理，从接收前 13 个字节时启用 ACK（告诉从设备继续发送）到接收最后一个字节前禁用 ACK 并发送停止条件（通知从设备数据接收完毕）。
f）Air780e.c
完整的代码如下：
void message_send (void)
{
    USARTx_SendString(UART5, "AT+CMGF=1\r\n");
    Delay_Ms(10);
    USARTx_SendString(UART5, "AT+CSCS=\"GSM\"\r\n");
    Delay_Ms(10);
    USARTx_SendString(UART5, "AT+CSMP=17,167,0,8\r\n");
    Delay_Ms(10);
    USARTx_SendString(UART5, "AT+CMGS=\"18272077924\"\r\n");
    Delay_Ms(100);

    // 发送短信内容（中文“用户摔倒”Unicode编码，UTF-16）
    uint8_t msg[] = {0x75, 0x28, 0x62, 0x37, 0x64, 0x54, 0x50, 0x12};
    for (int i = 0; i < sizeof(msg); i++) {
        USARTx_SendByte(UART5, msg[i]);
        Delay_Ms(10);
    }
    USARTx_SendByte(UART5, 0x1A);// 发送 Ctrl+Z (结束符)，16进制 0x1A
    Delay_Ms(10);
}
message_send() 利用 UART5 接口与 GSM 模块通信，发送一系列 AT 指令配置短信参数，并最终发送包含 "用户摔倒" 的求救短信到指定号码 18272077924。
1. AT+CMGF=1：启用文本模式（而非 PDU 模式），简化短信发送流程。
2. AT+CSCS="GSM"：设置字符集为 GSM，支持基本字符和部分扩展字符。
3. AT+CSMP=17,167,0,8：配置短信参数
  1. 短信类型（17）：发送短信（SMS-SUBMIT），带有效期字段。
  2. 有效期（167）：24 小时。
  3. 状态报告(0)：不需要接收方反馈送达状态。
  4. 编码方式(8)：UCS2（Unicode）编码，支持中文。
4. AT+CMGS="18272077924"：指定接收方手机号码。
5. \r\n：AT 指令的结束符。
g)   asr.c
该文件负责物体靠近警报、核心板和语音模块的串口信号和数据交互。
1. 物体靠近警报函数
每0.1s对各目标的速度取样一次，通过目标速度判定并更新目标速度状态，并使用消抖算法减少错误的警报：当识别到有目标0.5s内连续5次取样的（该次数为预设值，可修改)速度超过预设阈值时，才会触发物体靠近警报，使用位操作记录靠近目标组合（3个目标共7种组合），并向语音模块发送对应的警报信号。
uint8_t fall_flag = 0;//摔倒预警标识
volatile uint64_t last_time = 0;//用于周期性执行目标速度判定的时间戳
volatile uint64_t fall_time = 0;//记录摔倒预警首次触发的摔倒时间戳

// 默认报警阈值配置
AlarmConfig alarm_cfg =
{
        .speed_threshold = 110.0f,//速度阈值110 cm/s
        .debounce_count = 3      // 0.15秒防抖窗口 (0.05s*3，取样间隔0.05见fall_check函数）
};

// 三个目标的报警状态，默认为正常状态，超速计数器0
TargetAlarm target_alarms[3] =
{
    {ALARM_OFF, 0},
    {ALARM_OFF, 0},
    {ALARM_OFF, 0}
};

// 报警检测函数 (每0.05秒调用一次)
void Warn_Check(void)
{
    if(get_sys_time() - last_time < 500){
        return;
    }
    last_time = get_sys_time();

    uint8_t target_num = detect_flag ? 3 : 1;  // 根据单/多目标模式确定目标数量，detect_flag为1表示多目标
    uint8_t alarm_states = 0;  // 存储报警状态位图

    // 1. 更新所有目标状态
    for (int i = 0; i < target_num; i++) {
        float speed = Frame.groups[i].speed;//获取速度
        float threshold = alarm_cfg.speed_threshold;  // 使用预定义阈值

        switch (target_alarms[i].state) {
            case ALARM_OFF:
                if (speed < -threshold) {  // 目标超速//speed > threshold || speed < -threshold
                    target_alarms[i].state = ALARM_PENDING; //进入预警状态，超速计数器置一
                    target_alarms[i].counter = 1;
                }
                break;

            case ALARM_PENDING:
                if (speed < -threshold) {  // 目标超速//speed > threshold || speed < -threshold
                    if (++target_alarms[i].counter >= alarm_cfg.debounce_count) {
                        target_alarms[i].state = ALARM_ON;//超速计数器到达设定次数，进入报警状态
                    }
                } else {
                    if (target_alarms[i].counter > 0){
                        target_alarms[i].counter = 0;
                        target_alarms[i].state = ALARM_OFF;//预警状态下测得未超速，计数器置0，返回正常状态
                    }
                }
                break;

            case ALARM_ON:
                if (speed >= -threshold) {  //目标未超速
                    target_alarms[i].counter = 0;
                    target_alarms[i].state = ALARM_OFF;//报警状态下测得未超速，计数器置0，返回正常状态
                } else {
                    target_alarms[i].counter = alarm_cfg.debounce_count;//超速，状态和计数器不变
                }
                break;
        }
        // 记录报警状态(bit0-目标1, bit1-目标2, bit2-目标3)，将超速目标对应位置1
        if (target_alarms[i].state == ALARM_ON) {
            alarm_states |= (1 << i);
        }
    }

    // 2. 根据报警状态组合发送对应ID
    uint8_t send_id = 0;
    // 单目标模式只会发送目标1
    if (target_num == 1) {
        if (alarm_states & 0x01) {
            send_id = 1;
        }
    }else {// 三目标模式
        switch (alarm_states) {
            case 0x01: // 001仅目标1超速
                send_id = 1;
                break;
            case 0x02: // 010仅目标2超速
                send_id = 2;
                break;
            ......
            case 0x07: // 111全部超速
                send_id = 7;
                break;
        }
    }
    // 3. 向语音模块发送报警ID
    SendTargetId(send_id);
}
2. 其他函数
void float_to_bytes(float f, uint8_t *bytes, uint8_t max_len);//将浮点数转换为ASCII码方便数据发送
void dataRes(void);//识别USART2上来自语音模块的数据请求，并发送对应数据
void angSend(void);//发送转换过的待发送角度数据
void disSend(void);//发送转换过的待发送距离数据

//以下三个函数共同实现摔倒警报功能和相关的语音模块交互。摔倒预警后10s内不取消则会发送跌倒信息
void fall_send(void);//此函数在每次主循环都会调用，通过时间戳和跌倒预警标识实现预警播报后10s发送跌倒信息和对应的反馈信号
void fallsend_pre(void);//发送跌倒预警信号，仅在在第一次触发预警(触发时跌倒预警标识为0)时更新跌倒时间戳
void fallsend_cancel(void);//跌倒求救取消函数，重置跌倒标识并发送取消反馈。仅在核心板接收到语音版发送的取消求救信号时调用

void Warn_Init(void);//重置/初始化各目标的靠近警报状态和超速计数器
void SendTargetId(uint8_t target_id);//向语音模块发送物体靠近警报/跌倒预警/跌倒信息发送反馈/跌倒求救取消反馈
2.3.2 毫米波雷达模块 Rd-03D_V2
Rd-03D_V2毫米波雷达模块自带上位机工具 ICLM_MTT.exe 。在连接了上位机与 Rd-03D_V2硬件后， 上位机工具可以显示、 记录、 保存和回放雷达数据。
[图片]
图 2-17 ICLM_MTT界面
可视化工具界面主要分为“功能按钮区 1” “数据区 2” 和“目标显示区 3” ， 各区域功能如下:
1. 功能按钮区：“检测设备” 按钮用于检测 Rd-03D 模块是否连接成功；“开始/停止” 切换按钮用于开始或停止接收雷达数据；“区域监控” 按钮用于设置监控的区域以及设置盲区范围； “多目标检测/单目标检测” 切换按钮用于切换单目标检测和多目标检测模式； “检测回放/停止回放” 按钮用于回放已经录制的雷达数据；“数据保存/关闭保存” 按钮用于开启录制雷达数据；“保存数据” 按钮用于设置雷达数据的存放路径。
2. 数据区： 实时显示被跟踪目标的距离、 角度、 速度信息。
3. 目标显示区： 在雷达图中直观地显示被定位跟踪的目标在探测区域中的位置。
[图片]
图 2-18 单目标检测演示
[图片]
图 2-19 多目标检测演示
上位机还提供区域监控以及盲区设置功能。区域监控是指用户可以在检测区域内设定一个或多个感兴趣的多边形区域， 一旦目标进入该区域， 区域的颜色立即变化。
盲区设定是指用户可以设定感兴趣的雷达探测和追踪的范围， 关闭对某些距离门区域的检测和结果显示。
[图片]
图 2-20 “区域监控” 设置界面  
设置可见区域范围:默认是 0~23 距离门， 即无盲区。 用户可以自行设置最近端和最远端的盲区， 比如:设置 1~21， 即最近端设置 1 个距离门盲区， 最远端设置 2 个(23-21） 距离门盲区（单个距离门为 36 cm),点击“应用” 按钮后， 界面将如图 2.2.5 所示， 红色区域即为盲区位置。
安信可（Ai-Thinker）良好的软件生态也为我们后续扩展软件功能提供了良好的功能扩展基础。
在移动应用开发方面，可采用蓝牙/Wi-Fi双模通信架构。Android平台使用Java/Kotlin开发原生应用，iOS平台则用SwiftUI框架，实现雷达数据的实时可视化呈现。微信小程序方案可通过Web蓝牙API实现设备连接，配合WebSocket协议完成数据实时传输。界面设计应包含雷达数据可视化面板、目标轨迹绘制、接收信息处理显示等核心模块。
还可增加云端服务，终端设备通过MQTT协议接入物联网平台，业务逻辑层部署在云服务器，采用Spring Boot框架实现RESTful API。数据存储推荐时序数据库InfluxDB，便于处理时间序列数据。前端展示层可集成ECharts实现数据可视化，并支持多终端访问。
2.3.3 语音模块ASRPRO 2.0
语音模块通过两组串口和核心板实现指令和数据的交互。其中UART0负责语音模块发送数据请求、核心板返回对应数据；UART1负责串口指令的交互，如语音模块向核心板发送检测模式切换指令、核心板向语音模块发送跌倒预警、物体靠近警报等指令。软件设计中这两部分是独立的线程，为防止串口信号冲突使用了两组串口。
语音模块的软件设计主要由语音识别、持续获取核心板指令两个独立线程和其它函数组成。
a）语音识别程序
天问Block平台编程ASRPRO时，语音识别流程默认为唤醒模块后持续进行，用户仅需对预设的语音指令、语音指令识别结果对应的执行流程进行编程。个语音指令对应执行流程如下：
void ASR_CODE(){
//识别是否正在执行核心板指令
  while (wait == 1) {
    _nop_();
  }//wait为等待标识，核心板指令执行时置一，语音识别线程等待
  
  //对比语音指令识别结果，执行相应流程
  switch (snid) {
//语音模块测试指令1、2，开关板载LED灯:"打开/关闭灯光“
   case 1:
    digitalWrite(4,0);//PA4A引脚输出低电平，开灯
    break;
   case 2:
    digitalWrite(4,1);//PA4A引脚输出低电平，关灯
    break;
//指令3：“检测附近物体”。det_flag为单/多目标检测模式标识
   case 3:
    if(det_flag == 0){
      singbroad();//获取单目标检测结果并播报
    }else if(det_flag == 1){
      multibroad();//获取多目标检测结果并播报
    }
    break;
//指令4：“切换检测模式”
   case 4:
    Serial1.write(0x01);//UART1发送检测模式切换信号
    play_audio(10513);//语音播报{playid:10513,voice:切换指令已发送}
    det_flag = !det_flag;//修改检测模式标识
    break;
//指令5：“取消”（取消跌倒求救）
   case 5:
    //判断是否进入跌倒预警状态，fall_flag为跌倒预警标识
    if(fall_flag == 1){
      Serial1.write(0x02);//如果为跌倒预警状态，UART1向核心板发送取消求救指令
      play_audio(10514);//语音播报{playid:10514,voice:取消指令已发送}
    }else{
      play_audio(10515);//语音播报{playid:10515,voice:当前未检测到跌倒}
    }
    break;
//指令6、7：“提高/降低音量”。音量范围为1-7
//提高音量
   case 6:
   //判断是否到达上限
    if(loud + 1 > 7){
      play_audio(10516);//语音播报{playid:10516,voice:已经是最高音量}
    }else{
      loud = loud + 1;
      vol_set(loud);//改变音量变量并配置音量
      play_audio(10517);//语音播报{playid:10517,voice:音量已提高至}
      play_num((int64_t)(loud * 100), 1);//语音播报音量值
    }
    break;
//降低音量，代码逻辑同上，具体代码略
   case 7:
    ......
  }
}
b）获取核心板指令
每10ms从核心板获取一次警报信号，根据警报信号类别执行相应进程。
void warn(){
  while (1) {
    //1、2、3表示对应单个目标初速;4、5、6一次表示3、2、1目标未超速，其他目标超速；7表示三个目标全超速。
    //8为摔倒预警，9为摔倒求救已经发送的反馈信号，10为摔倒警报解除的反馈信号
    //11为上电播报；12、13为检测模式切换反馈，分别对应单、多目标检测模式
    //0为无警报信号
    hit = ((uint8_t)(String(Serial1.readString()).toInt()));//UART1获取警报信号
//是否有警报、是否为物体靠近警报
    if(hit != 0){
      if(hit <= 8){
        play_audio(10500);//语音播报{playid:10500,voice:警报}
      }
//判断警报目标
      switch (hit) {
//警报1-7：物体靠近警报。warn1-warn3函数作用为获取并播报对应目标位置
       case 1:
        warn1();//播报目标1位置
        play_audio(10501);//语音播报{playid:10501,voice:有物体靠近，注意避让}
        break;
       ......//case2-6省略
       case 7:
        warn1();    warn2();    warn3();//依次播报目标1-3位置
        play_audio(10507);//语音播报{playid:10507,voice:有物体靠近，注意避让}
        break;
//摔倒预警
       case 8:
        play_audio(10508);//语音播报{playid:10508,voice:用户跌倒！}
        play_audio(10509);//语音播报{playid:10509,voice:如果您不需要求救，请在}
        play_num((int64_t)(cancel_time * 100), 1);//播报预设求救取消时限数值，设置为10(s)
        play_audio(10510);//语音播报{playid:10510,voice:秒内取消}
        fall_flag = 1;//跌倒预警标识置一
        //非阻塞延时，暂停警报信号接收，减少其他警报信号干扰。提前将等待标识置零，保证不阻塞语音识别线程
        //特别是“取消求救”指令
        wait = 0;
        delay(3990);
        break;
//摔倒求救取消反馈
       case 9:
        play_audio(10511);//语音播报{playid:10511,voice:跌倒求救已经发送}
        fall_flag = 0;//摔倒预警标识置零
        break;
//摔倒求救已发送反馈
       case 10:
        play_audio(10512);//语音播报{playid:10512,voice:跌倒求救解除}
        fall_flag = 0;//摔倒预警标识置零
        break;
//上电播报
       case 11:
        ......
//检测模式切换反馈-单目标。修改工作模式标识并播报
       case 12：
        ......
//检测模式切换反馈-多目标。修改工作模式标识并播报
       case 13：
        ......
      }
      dataclear();//清楚各目标数据的函数
      wait = 0;//清除等待标识
      hit = 0;//清除警报信号
    }
    delay(10);//非阻塞延时，UART1每10ms获取一次警报信号
  }
}
c）其它函数
void warn1();//warn1-warn3获取并播报对应目标位置
void warn2();
void warn3();

float getdata(float x, uint8_t order);//UART0发送请求，获取目标数据。x为目标数据变量，order为目标数据对应的请求信号
void dataclear();//清除各目标数据变量
void serialclear();//清除串口接收缓冲区

void broad_cm(float dis, float ang);//以厘米为单位播报目标位置，dis和ang分别为目标的距离和角度
void broad_m(float dis, float ang);//以米为单位播报目标位置，dis和ang分别为目标的距离和角度
void singbroad();//检测附近静止物体的单目标播报
void multibroad();//检测附近静止物体的多目标播报
第三部分  完成情况与性能参数
3.1 整体介绍
整个系统实物由腰带、外壳、电路和电源四个部分构成，它们共同组成一个完整的穿戴式设备。
腰带作为整个系统的基础载体，具备舒适性和可调节性，采用轻便柔韧的材料，同时预留固定外壳的接口，确保稳定佩戴。
外壳作为保护层，覆盖在电路外部，起到防水、防尘和抗冲击的作用，采用ABS塑料材质，既保证耐用性又兼顾轻量化，其结构可以是分体式以便维修或一体式以增强密封性，并通过螺丝、卡扣或磁吸方式固定在腰带上。  
电路部分是整个系统的核心，包含主控芯片、传感器、通信模块等功能单元，负责数据采集、处理和传输。电源则为电路提供能量，采用可充电锂电池，由系统平均功率可知，5000mA·h的电池可供电25h左右，电源被设计成便于更换的形式，嵌入腰带夹层中。  
这四个部分相互配合，腰带提供穿戴支撑，外壳确保内部组件的安全，电路实现功能控制，电源维持系统运行，最终形成一个稳定、高效且用户友好的完整系统。
[图片]
[图片]
                         图 3-1 系统外观图                                                      图 3-2 内部电路情况
3.2 工程成果
3.2.1 机械成果
使用ABS材质外壳，尺寸为150×110×70mm，具有IP65级防护标准，防水防尘，对内部电路具有良好的保护性，同时ABS材料不会对电磁信号屏蔽或者干扰，确保传感器和通信模块的正常稳定运行。
腰带连接固定，后期可专门为用户身体尺寸定制腰带，符合人体工程学，实现良好固定，腰带上附加多个储物袋，可放置便携式电源或者外置麦克风和喇叭，也可存放用户日常杂物。
3.2.2 软件成果
采用MounRiver Studio编写、编译、烧写程序。MounRiver Studio（MRS）是一款专注于RISC-V架构芯片开发的集成开发环境。该IDE基于Eclipse框架构建，提供代码编辑、编译、调试和烧录等全流程开发功能，支持多种RISC-V内核的微控制器，支持代码自动补全、语法高亮和错误检查，同时提供沁恒芯片的工程模板，简化初始化流程。
本系统采用主从式开发架构，以system文件为主控核心，调用各个外设函数接口，通过规范化的外设驱动处理文件作为硬件接口层，实现分层化、模块化的软件设计。在程序架构方面，采用中断服务与主程序分优先级并行的处理机制，通过合理的中断嵌套管理确保关键任务的实时响应性，同时保证主程序流程的高效执行。
在代码组织规范上，严格遵循以下原则：首先，按照功能模块划分文件结构，确保每个变量和函数都存放在其逻辑所属的文件中；其次，采用具有明确意义的命名规范，变量和函数命名需直观反映其功能和用途；第三，对于需要跨文件访问的变量，统一使用extern关键字进行声明，以维护良好的模块边界。
在算法优化方面，针对嵌入式系统的性能特点实施多项优化措施：使用乘法运算替代耗时的除法操作；在适当场景采用高效的位操作代替常规算术运算；对可能被异步修改的关键变量添加volatile修饰符，确保编译器不会进行错误的优化。
3.3 特性成果
3.3.1 附近物体检测功能
这是项目基于毫米波雷达实现的一定范围内单/多个静止/运动物体方位检测功能。该功能可通过语音指令控制语音模块向核心板传达切换检测模式指令，再通过核心板控制雷达切换检测模式。雷达时刻向核心板发送检测到的目标方位数据，并交由核心板进行数据处理得到坐标和距离、角度。语音模块在接收到“检测附近物体”指令时，向核心板请求最新的检测结果并播报。
检测范围为水平方位角±60° ， 俯仰角±30°，最大检测距离8m，探测精度0.1m，最大检测目标数3。
3.3.2 跌倒警报功能
该功能通过mpu6050陀螺仪模块时刻监测加速度、角速度，并在两者超过正常范围时进行姿态角计算，实现跌倒判定。若在跌倒预警播报后10s内未通过语音指令取消，将通过移动通信模块air780e向预设手机号发送跌倒求救信息。
跌倒识别的加速度范围为低于0g（自由落体时加速度接近0g，g为重力加速度）或高于2.1g（落地冲击时加速度可达2.1g），角速度阈值为22.9°/s，0.5s后姿态角判定阈值为20° 。跌倒响应时间约0.5s，取消时间10s。
3.3.3 物体靠近警报功能
项目基于毫米波雷达实时监测到的附近目标的速度值、含数据消抖算法的阈值判决来实现物体靠近警报。基于雷达当前的工作模式可实现1-3个目标的靠近警报。
检测范围同附近物体检测功能为水平方位角±60° ， 俯仰角±30°，最大检测距离8m，最大警报目标数3。速度阈值为1.1m/s。目标速度采样时间间隔为0.05s，采样速度连续超过阈值的次数达到3次后进行警报。响应时间在2s内。
3.3.4 语音交互和蓝牙功能
项目基于ASRPRO2.0语音模块，构建了一套完善的语音交互流程；项目包含蓝牙模块，可以使用蓝牙模块和蓝牙耳机进行配对，使用蓝牙耳机进行播报。
第四部分  总结与展望
4.1 可扩展之处
a）功能扩展：可进一步集成 GPS 模块，实现定位导航功能，结合地图数据为盲人用户提供更详细的路线指引，短信指令可发送准确定位提高安全性；增加环境感知传感器，如温湿度传感器、空气质量传感器等，为用户提供更多环境信息。
b）算法优化：持续优化避障算法与跌倒检测算法，引入机器学习、深度学习技术，提高算法对复杂环境与特殊情况的处理能力，提升系统智能化水平。
c）通信升级：考虑升级通信模块，采用 5G 通信技术，提高数据传输速度与稳定性，支持远程监控与数据上传，便于监护人实时了解用户状态。
d）外观设计：优化系统硬件结构与外观设计，使其更加轻便、舒适，便于盲人用户携带与使用；增加防水、防尘功能，提高产品在不同环境下的适应性。
4.2 心得体会
4.2.1 CH32V307VCT6供电的问题
在开发项目的过程中，发现了一个有可能有问题的现象，关闭CH32V307VCT6的开关时，开发板的5V引脚仍有电压。在系统硬件连接时，为了节省操作流程，各个外设均是通过主控板的3V3引脚或者5V引脚供电的，在一次连接语音模块的过程中，由于语音板SPK模块的电路保护不足，并且我们不知道开发板的5V引脚在开关关闭的情况下也有电压，导致SPK电路烧毁。
查阅资料分析原因得知：标准的 USB 接口（Type-A, Type-C 等）都包含一条名为 VBUS 的电源线。将开发板的 USB 接口连接到主机时，主机就会通过 VBUS 线向设备提供 +5V 电压（USB 规范标准）。这个 VBUS 电压是独立于开发板上可控制的开关（主电源）存在的。只要 USB 线插着，并且主机在供电，VBUS 上就有 5V。
原因是在开发板上，芯片的 USB_DP (D+) 和 USB_DM (D-) 数据线必须连接到 USB 连接器的对应引脚。并且芯片的 USB_VBUS 引脚通常也会通过一个小的限流电阻连接到 USB 连接器的 VBUS 引脚。这是为了让芯片能够检测到主机的存在（USB 插入检测）。
为了保护敏感的 USB PHY（物理层接口）电路免受静电放电（ESD）的损坏，芯片内部通常在 USB_VBUS 引脚和芯片的主电源轨 (VDD / VCC) 之间集成 ESD 保护二极管。当开发板主电源开关断开（即 VCC / VDD 网络被切断），但 USB 线仍然连接着主机（VBUS 有 +5V）时，会发生如下情况：USB_VBUS 引脚被主机强行拉到接近 +5V，芯片内部的 VDD 网络（原本由主电源供电）现在是“悬空”的（或者通过一些电阻连接到地）。这时，芯片内部连接在 USB_VBUS 和 VDD 之间的那个 ESD 保护二极管就变成了正向偏置，USB VBUS (+5V) 的电流会通过这个导通的二极管，反向流入到原属于 VDD 的网络中，这样，在开发板上连接到 VDD / VCC 网络的点就能测量到一个电压。这个电压大约是5V电压减去二极管正向偏置的压降。
4.2.2 语音模块SPK芯片烧毁问题
在各模块整合的过程中，我们曾两次烧毁ASRPRO2.0语音模块的SPK（语音输出）芯片。经过对电路和烧毁时操作的分析，我们找出了SPK芯片烧毁原因，并采取了相应措施防止该情况再次发生。
a）第一次烧毁
在一次联调时，我们使用 CH32 的5V输出引脚为语音模块供电，并连接了两组需要使用的串口和地线。之后，我们将 USB 接入 CH32 的调试器接口，准备进行调试。但由于前述原因5V引脚仍有电压，在 USB 插入瞬间，调试器开关未打开、核心板尚未进入工作状态的情况下，语音模块上电播报，紧接着 SPK 芯片很快烧毁。
对 ASRPRO2.0 电路原理图的分析后，我们发现SPK芯片的保护电路不够完善。它的 VCC 和 GND 之间仅有一个 4.7uF 电容，只能抑制毫秒级的电压跌落，无法抑制插拔USB时产生的纳秒级电压尖峰；没有TVS管，无法抑制5V系统上电时可能产生的7.2V上电浪涌。这就导致SPK芯片被USB插拔瞬间产生的电压尖峰击穿。
由于SPK芯片电路集成在语音模块里，我们无法为其添加TVS管等保护措施。为防止该种情况再次发生，在之后的联调过程中，我们使用语音模块的调试器USB接口（拥有充足的保护电路）为语音模块提供供电，或在核心板上电、开机后再将其5V输出接入语音模块。
b）第二次烧毁
在另一次联调时，我们采用语音模块自身的 USB 接口为其供电，与核心板仅有地线和两组串口连接。语音模块维持上电状态，在保持和核心板连接的情况下，由核心板调试器向核心板烧写新的程序。此过程中语音模块SPK芯片烧毁。
查阅资料后发现，这种连接方式会形成闭合地线环路，而 CH32 板载调试器 WCH-LinkE 向核心板烧写程序时会导致高频的 SWD 时钟信号和高速数据输入，产生高频开关噪声。这样的共模瞬态高压通过地线环路进入语音模块，导致保护电路不足的SPK芯片被击穿。
在无法为SPK芯片添加独立保护电路的条件下，为防止此类事故再次发生，我们在每次修改核心板程序、准备烧录时，会断开语音模块和核心板的地线连接。
