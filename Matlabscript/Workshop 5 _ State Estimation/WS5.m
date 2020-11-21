data = csvread('sensor_data.csv');
int_con = csvread('initial_conditions.csv');
t = data(:,1);%ใช้ค่าหลักที่ 1 data(:,หลัก) found 100 samples
N = numel(t); % numel หาขนาดของ vector หรือ matrix นั้น
dt = t(2) - t(1);
z = data(:,2:end)% sensor data เอาค่า ทุกแถวในหลักที่ 2 ถึงสุดท้าย
%% setup Kalman filter

x_0 = int_con; % initial condition
P_0 = diag([1 1 1 1]); % initial covariance of estimated states
F = [1 dt 0 0; 0 1 0 0 ; 0 0 1 dt ; 0 0 0 1];
B = eye(4);
G = [0.5*dt^2 0; dt 0 ; 0 0.5*dt^2; 0 dt];
H = eye(4);
D = zeros(4);
g =9.80665;
u = repmat([0 0 -0.5*-g*dt^2 dt],[N,1]);
Q = diag([0.1 0.1]) % covariance matrix of process
R = diag ([1 1 1 1]) % covariance matrix of measurement
n = 4; % number of states

x_est = zeros(n,N);
x_est(:,1) = reshape(x_0,[],1);
P_est = zeros(n,n,N);
P_est(:,:,1) = P_0;

for i = 1:N-1
    x_predict = F*x_est(:,i)+B*u(i,:)';
    P_predict = F*P_est(:,:,i)*F'+G*Q*G';
    z_predict = H*x_predict+D*u(i,:)';
    
    S = H*P_predict*H'+R;
    K = P_predict*H'/S;
    x_est(:,i+1) = x_predict+K*(z(i,:)'-z_predict);
    P_est(:,:,i+1)= (eye(n)-K*H)*P_predict;
end
x_est = x_est';
hold on 
plot(z(:,1),z(:,3),'r.') % plot x เทียบ y  zตัวแรกเป็น x
plot(x_est(:,1),x_est(:,3),'b','linewidth',3)
axis equal % ทำให้ แกน 2 แกน ใช้ สเกล เดียวกัน 