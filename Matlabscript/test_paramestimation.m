%%  import data
z = iddata(table2array(ChirpSignalEX(:,5)),table2array(ChirpSignalEX(:,3)),0.002);%ChirpSignalRespondRSpro
z.InputName = 'Voltage';
z.InputUnit =  'V';
z.OutputName = {'Angular position'};
z.OutputUnit = {'rad'};
z.Tstart = 0;
z.TimeUnit = 's';
figure('Name', [z.Name ': Voltage input -> Angular position output']);% Plot first input-output pair (Voltage -> Angular position).
plot(z(:, 1, 1)); 
%% set initial 
FileName      = 'Vilgalaxy';       % File describing the model structure.
Order         = [1 1 2];           % Model orders [ny nu nx].
Parameters    = [100;100];    % Initial parameters.1.0;0.2;2.0;0.05;0.05;0.001 
InitialStates = [0; 0];            % Initial initial states.
Ts            = 0;                 % Time-continuous system.

nlgr = idnlgrey(FileName, Order, Parameters, InitialStates, Ts,'Name','Vilgalaxy');
nlgr = setpar(nlgr,'Minimum',{0 0});
set(nlgr, 'InputName', 'Voltage', 'InputUnit', 'V',               ...
          'OutputName', {'Angular position'}, ...
          'OutputUnit', {'rad'},                         ...+
          'TimeUnit', 's');
nlgr = setpar(nlgr, 'Name', {'par1' 'par2'});
%% set Tolerance estimation
nlgr.SimulationOptions.AbsTol = 1e-7;
nlgr.SimulationOptions.RelTol = 1e-7;
compare(z, nlgr);
%% RUN parameters estimation
nlgr = setinit(nlgr, 'Fixed', {true true}); % Estimate the initial states.
opt = nlgreyestOptions('Display', 'on');
opt.SearchOptions.MaxIterations = 50;
nlgr = nlgreyest(z, nlgr, opt);
compare(z,nlgr);