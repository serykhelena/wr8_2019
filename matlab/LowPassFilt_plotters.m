global dat
delete(instrfind);
dat = serial('COM7', 'BaudRate', 115200);
dat.InputBufferSize = 4096;

fopen(dat)
set(dat, 'ByteOrder', 'littleEndian')

disp 'Ok!'

A=[];
Adc = [];
LowPass09 = [];
LowPass05 = [];
LowPass01 = [];
LowPass001 = [];
time = [];
t = 1000;
 for i = 1:t
    A=fread(dat, [5,1], 'int32');
    time = [time;i*10]; % every 10 ms 1 value
    Adc = [Adc; A(1)];
    LowPass09 = [LowPass09; A(2)];
    LowPass05 = [LowPass05; A(3)];
    LowPass01 = [LowPass01; A(4)];
    LowPass001 = [LowPass001; A(5)];
 end
fclose(dat);
plot(time,Adc)
xlabel('t,мс')
ylabel('U,B')
figure
plot(time,LowPass09)
xlabel('t,мс')
ylabel('U,B')
figure
plot(time,LowPass05)
xlabel('t,мс')
ylabel('U,B')
figure
plot(time,LowPass01)
xlabel('t,мс')
ylabel('U,B')
figure
plot(time,LowPass001)
xlabel('t,мс')
ylabel('U,B')
disp 'Close'