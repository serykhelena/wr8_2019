instrreset
global igotit

igotit = serial('COM11','BaudRate',115200);
igotit.InputBufferSize = 4096;

fopen(igotit);
set(igotit,'ByteOrder','littleEndian');

disp 'Ok!'

tline = [];
array = [];
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
fwrite(igotit, start, 'int8')
for i = 1:count
    tline = fread(igotit, [500,1], 'int16');
    array = [array; tline];
end

start = 'f'; %forward +5
fwrite(igotit, start, 'int8');
for i = 1:count
    tline = fread(igotit, [500,1], 'int16');
    array = [array; tline];
end

% start = 'v'; %backward -5
% fwrite(igotit, start, 'int8');
% for i = 1:count
%     tline = fread(igotit, [500,1], 'int16');
%     array = [array; tline];
% end

start = ' ';
fwrite(igotit, start, 'int8')
for i = 1:count
    tline = fread(igotit, [500,1], 'int16');
    array = [array; tline];
end

% start = 'g'; %forward +10
% fwrite(igotit, start, 'int16');
% for i = 1:count
%     tline = fread(igotit, [500,1], 'int16');
%     array = [array; tline];
% end
% 
% start = 'b'; %backward -10
% fwrite(igotit, start, 'int16');
% for i = 1:count
%     tline = fread(igotit, [500,1], 'int16');
%     array = [array; tline];
% end
% 
% start = 'h'; %forward +15
% fwrite(igotit, start, 'int16');
% for i = 1:count
%     tline = fread(igotit, [500,1], 'int16');
%     array = [array; tline];
% end
% 
% 
% start = 'n'; %backward -15
% fwrite(igotit, start, 'int16');
% for i = 1:count
%     tline = fread(igotit, [500,1], 'int16');
%     array = [array; tline];
% end
% 
% start = 'j'; %forward +20
% fwrite(igotit, start, 'int16');
% for i = 1:count
%     tline = fread(igotit, [500,1], 'int16');
%     array = [array; tline];
% end
% 
% start = 'm'; %backward -20
% fwrite(igotit, start, 'int16');
% for i = 1:count
%     tline = fread(igotit, [500,1], 'int16');
%     array = [array; tline];
% end

fclose(igotit);
disp 'Done!'

for i = 1:2:(length(array))
    real5 = [real5; array(i,1)];
end

% for i = (3000-1):2:(5000-1)
%     real10 = [real10; array(i,1)];
% end
% 
% for i = (5000-1):2:(7000-1)
%     real15 = [real15; array(i,1)];
% end
% 
% for i = (7000-1):2:(9000-1)
%     real20 = [real20; array(i,1)];
% end

for j = 2:2:(length(array)-1)
    ref5 = [ref5; array(j,1)];
end

% for j = 3000:2:(5000)
%     ref10 = [ref10; array(j,1)];
% end
% 
% for j = 5000:2:(7000)
%     ref15 = [ref15; array(j,1)];
% end
% 
% for j = 7000:2:(9000)
%     ref20 = [ref20; array(j,1)];
% end

grid on;
hold on;
plot(real5,'r')
% plot(real10,'k')
% plot(real15,'g')
% plot(real20)
plot(ref5,'c')
% plot(ref10,'m')
% plot(ref15,'y')
% plot(ref20,'b')

delete(igotit);
clear igotit
