global dat

dat = serial('COM11','BaudRate',115200);
dat.InputBufferSize = 4096;

fopen(dat);
set(dat,'ByteOrder','littleEndian');

disp 'Connection - done!'

temp = [];
all_data = [];
count = 2;

ref5 = [];
ref10 = [];
ref15 = [];
ref20 = [];

real5 = [];
real10 = [];
real15 = [];
real20 = [];

start = 'o';
fwrite(dat, start, 'int8')

for i = 1:count
    temp = fread(dat, [100,1], 'int16');
    all_data = [all_data; temp];
end

start = 'h'; %forward +15
disp 'Go forward!'
fwrite(dat, start, 'int16');
for i = 1:count
    temp = fread(dat, [200,1], 'int16');
    all_data = [all_data; temp];
end
disp 'Go backward!'
start = 'n'; %backward -10
fwrite(dat, start, 'int16');
for i = 1:count
    temp = fread(dat, [200,1], 'int16');
    all_data = [all_data; temp];
end

start = ' ';
fwrite(dat, start, 'int8')
for i = 1:count
    temp = fread(dat, [100,1], 'int16');
    all_data = [all_data; temp];
end

fclose(dat);
disp 'Connection - closed'

for i = 1:2:(length(all_data)-1)
    real10 = [real10; all_data(i,1)];
    ref10 = [ref10; all_data(i+1,1)];
end

% for i = (3000-1):2:(5000-1)
%     real10 = [real10; array(i,1)];
% end
% 
% for i = (5000-1):2:(7000-1)
%     real15 = [real15; array(i,1)];
% end

% for i = (7000-1):2:(9000-1)
%     real20 = [real20; array(i,1)];
% end

% for j = 3000:2:(5000)
%     ref10 = [ref10; array(j,1)];
% end

% for j = 5000:2:(7000)
%     ref15 = [ref15; array(j,1)];
% end

% for j = 7000:2:(9000)
%     ref20 = [ref20; array(j,1)];
% end

grid on;
hold on;
plot(real10)
% plot(real10,'k')
% plot(real15,'g')
% plot(real20)
plot(ref10,'r')
% plot(ref10,'m')
% plot(ref15,'y')
% plot(ref20,'b')

% delete(igotit);
% clear igotit