global dat
delete(instrfind);
dat = serial('COM7', 'BaudRate', 115200);
dat.InputBufferSize = 4096;

fopen(dat)
set(dat, 'ByteOrder', 'littleEndian')
fwrite(dat,' ','char');
fwrite(dat,'q','char');
disp 'Ok!'
A=[];
B = [];
 for i = 1:500
    A=fread(dat, [4,1], 'int32');
    A = A';
    B = [B;A];
 end
 fwrite(dat,'a','char');
  for i = 501:1000
    A=fread(dat, [4,1], 'int32');
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
Average = B(:,2);
Median = B(:,3);
Combo = B(:,4);
disp 'Close'
plot(time,Adc)
xlabel('t,мс')
ylabel('U,B')
koef = 0.0008056640625;
Adc = koef * Adc;
Average = koef * Average;
Median = koef * Median;
Combo = koef * Combo;
plot(time,Adc,time,Average)
xlabel('t,мс')
ylabel('U,B')
figure
plot(time,Adc,time,Median)
xlabel('t,мс')
ylabel('U,B')
figure
plot(time,Adc,time,Combo)
xlabel('t,мс')
ylabel('U,B')
figure
plot(time,Adc)
xlabel('t,мс')
ylabel('U,B')