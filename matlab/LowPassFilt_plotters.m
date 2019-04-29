global dat
delete(instrfind);
dat = serial('COM7', 'BaudRate', 115200);
dat.InputBufferSize = 4096;

fopen(dat)
set(dat, 'ByteOrder', 'littleEndian')
fwrite(dat,' ','char');
fwrite(dat,'q','char');
disp 'Ok!'

A = [];
B = [];
 for i = 1:500
    A=fread(dat, [5,1], 'int32');
    A = A';
    B = [B;A];
 end
 fwrite(dat,'a','char');
  for i = 501:1000
    A=fread(dat, [5,1], 'int32');
    A = A';
    B = [B;A];
  end
fwrite(dat,' ','char');
fclose(dat);
time = [];
  for i = 1:1000
    time = [time; i*10];
  end

Adc = B(:,1);
LowPass09 = B(:,2);
LowPass05 = B(:,3);
LowPass01 = B(:,4);
LowPass001 = B(:,5);
disp 'Close'
plot(time,Adc)
xlabel('t,мс')
ylabel('U,B')
koef = 0.0008056640625;
Adc = koef * Adc;
LowPass09 = koef * LowPass09;
LowPass05 = koef * LowPass05;
LowPass01 = koef * LowPass01;
LowPass001 = koef * LowPass001;
figure
plot(time,Adc,time,LowPass09)
xlabel('t,мс')
ylabel('U,B')
figure
plot(time,Adc,time,LowPass05)
xlabel('t,мс')
ylabel('U,B')
figure
plot(time,Adc,time,LowPass01)
xlabel('t,мс')
ylabel('U,B')
figure
plot(time,Adc,time,LowPass001)
xlabel('t,мс')
ylabel('U,B')