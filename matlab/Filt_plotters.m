global dat
delete(instrfind);
dat = serial('COM7', 'BaudRate', 115200);
dat.InputBufferSize = 4096;

fopen(dat)
set(dat, 'ByteOrder', 'littleEndian')

disp 'Ok!'

A=[];
Adc = [];
Average = [];
Median = [];
Combo = [];
time = [];
t = 1000;
 for i = 1:t
    A=fread(dat, [4,1], 'int32');
    time = [time;i*10]; % every 10 ms 1 value
    Adc = [Adc; A(1)];
    Average = [Average; A(2)];
    Median = [Median; A(3)];
    Combo = [Combo; A(4)];
 end
fclose(dat);
plot(time,Adc)
xlabel('t,мс')
ylabel('U,B')
figure
plot(time,Average)
xlabel('t,мс')
ylabel('U,B')
figure
plot(time,Median)
xlabel('t,мс')
ylabel('U,B')
figure
plot(time,Combo)
xlabel('t,мс')
ylabel('U,B')
disp 'Close'