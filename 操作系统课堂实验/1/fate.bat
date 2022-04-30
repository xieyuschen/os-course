@echo off
 set /p yearmonthday=Input Birthday (For example 19990411)
 set "shengxiao=Monkey Cock Dog Pig Mouse Cow Tiger Rabbit Drogen Snake House Sheep"
 set year=%yearmonthday:~0,4%
 set monthday=%yearmonthday:~4,4%
 set /a mod=%year%%%12



if %mod%==0 Echo Monkey
 if %mod%==1 Echo Cock
 if %mod%==2 Echo Dog
 if %mod%==3 Echo Pig
 if %mod%==4 Echo Mouse
 if %mod%==5 Echo Cow
 if %mod%==6 Echo Tiger
 if %mod%==7 Echo Rabbit
 if %mod%==8 Echo Drogen
 if %mod%==9 Echo Snake
 if %mod%==10 Echo House
 if %mod%==11 Echo Sheep
 echo is


if "%monthday%" LEQ "0119" echo Moxie
 if "%monthday%" GEQ "0120" if "%monthday%" LEQ "0218" echo ShuiPing
 if "%monthday%" GEQ "0219" if "%monthday%" LEQ "0320" echo ShuangYu
 if "%monthday%" GEQ "0321" if "%monthday%" LEQ "0419" echo BaiYang
 if "%monthday%" GEQ "0420" if "%monthday%" LEQ "0520" echo JinNiu
 if "%monthday%" GEQ "0521" if "%monthday%" LEQ "0621" echo ShuangZi
 if "%monthday%" GEQ "0622" if "%monthday%" LEQ "0722" echo JuXie
 if "%monthday%" GEQ "0723" if "%monthday%" LEQ "0822" echo Shizi
 if "%monthday%" GEQ "0823" if "%monthday%" LEQ "0922" echo ChuNv
 if "%monthday%" GEQ "0923" if "%monthday%" LEQ "1023" echo TianCheng
 if "%monthday%" GEQ "1024" if "%monthday%" LEQ "1122" echo TianXie
 if "%monthday%" GEQ "0321" if "%monthday%" LEQ "0419" echo BaiYang
 if "%monthday%" GEQ "1222" echo MoXie
  
 Pause