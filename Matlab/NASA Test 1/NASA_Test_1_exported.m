classdef NASA_Test_1_exported < matlab.apps.AppBase

    % Properties that correspond to app components
    properties (Access = public)
        UIFigure                        matlab.ui.Figure
        GridLayout                      matlab.ui.container.GridLayout
        LeftPanel                       matlab.ui.container.Panel
        SetupPanel                      matlab.ui.container.Panel
        GridLayout2                     matlab.ui.container.GridLayout
        BasicChecksPanel                matlab.ui.container.Panel
        GridLayout3                     matlab.ui.container.GridLayout
        CheckButton                     matlab.ui.control.Button
        Lamp                            matlab.ui.control.Lamp
        EditField                       matlab.ui.control.EditField
        WiringCheckLabel                matlab.ui.control.Label
        TubingCheckLabel                matlab.ui.control.Label
        CheckButton_2                   matlab.ui.control.Button
        Lamp_2                          matlab.ui.control.Lamp
        ManualCheckLabel                matlab.ui.control.Label
        FlowSettingsPanel               matlab.ui.container.Panel
        GridLayout7                     matlab.ui.container.GridLayout
        O2Label                         matlab.ui.control.Label
        O2Spinner                       matlab.ui.control.Spinner
        N2SpinnerLabel                  matlab.ui.control.Label
        N2Spinner                       matlab.ui.control.Spinner
        WeightScaleResetPanel           matlab.ui.container.Panel
        GridLayout5                     matlab.ui.container.GridLayout
        ResetScaleButton                matlab.ui.control.Button
        InitialWeightLabel              matlab.ui.control.Label
        gEditFieldLabel                 matlab.ui.control.Label
        gEditField                      matlab.ui.control.NumericEditField
        ChamberBackfillPanel            matlab.ui.container.Panel
        GridLayout6                     matlab.ui.container.GridLayout
        EvacuateChamberButton           matlab.ui.control.Button
        kPaEditFieldLabel               matlab.ui.control.Label
        kPaEditField                    matlab.ui.control.NumericEditField
        PressureLabel                   matlab.ui.control.Label
        RefillChamberButton             matlab.ui.control.Button
        Lamp_3                          matlab.ui.control.Lamp
        Lamp_4                          matlab.ui.control.Lamp
        StartRunPanel                   matlab.ui.container.Panel
        GridLayout8                     matlab.ui.container.GridLayout
        IgnitionButton                  matlab.ui.control.Button
        Lamp_5                          matlab.ui.control.Lamp
        RetractIgniterButton            matlab.ui.control.Button
        Lamp_6                          matlab.ui.control.Lamp
        CenterPanel                     matlab.ui.container.Panel
        LivestreamPanel                 matlab.ui.container.Panel
        TabGroup                        matlab.ui.container.TabGroup
        LiveGraphsTab                   matlab.ui.container.Tab
        GridLayout14                    matlab.ui.container.GridLayout
        TabGroup2                       matlab.ui.container.TabGroup
        TemperatureTab                  matlab.ui.container.Tab
        GridLayout15                    matlab.ui.container.GridLayout
        UIAxes                          matlab.ui.control.UIAxes
        RadiativeFluxTab                matlab.ui.container.Tab
        GridLayout15_2                  matlab.ui.container.GridLayout
        UIAxes_2                        matlab.ui.control.UIAxes
        ChangeinWeightTab               matlab.ui.container.Tab
        GridLayout15_3                  matlab.ui.container.GridLayout
        UIAxes2                         matlab.ui.control.UIAxes
        LiveTables                      matlab.ui.container.Tab
        GridLayout16                    matlab.ui.container.GridLayout
        UITable                         matlab.ui.control.Table
        RightPanel                      matlab.ui.container.Panel
        VitalsWarningsPanel             matlab.ui.container.Panel
        GridLayout10                    matlab.ui.container.GridLayout
        PressureSensorPanel             matlab.ui.container.Panel
        GridLayout11                    matlab.ui.container.GridLayout
        ChamberPressureEditFieldLabel   matlab.ui.control.Label
        ChamberPressureEditField        matlab.ui.control.NumericEditField
        kPaLabel                        matlab.ui.control.Label
        Lamp_7                          matlab.ui.control.Lamp
        SensorTemperatureEditFieldLabel  matlab.ui.control.Label
        SensorTemperatureEditField      matlab.ui.control.NumericEditField
        KLabel                          matlab.ui.control.Label
        Lamp_8                          matlab.ui.control.Lamp
        PaperIgnitedPanel               matlab.ui.container.Panel
        GridLayout12                    matlab.ui.container.GridLayout
        PaperIgnitedCheckBox            matlab.ui.control.CheckBox
        MassFlowControllersPanel        matlab.ui.container.Panel
        GridLayout13                    matlab.ui.container.GridLayout
        VolumetricFlowO2EditFieldLabel  matlab.ui.control.Label
        VolumetricFlowO2EditField       matlab.ui.control.NumericEditField
        ULabel                          matlab.ui.control.Label
        VolumetricFlowN2EditFieldLabel  matlab.ui.control.Label
        VolumetricFlowN2EditField       matlab.ui.control.NumericEditField
        ULabel_2                        matlab.ui.control.Label
        MassFlowO2EditFieldLabel        matlab.ui.control.Label
        MassFlowO2EditField             matlab.ui.control.NumericEditField
        MassFlowN2EditFieldLabel        matlab.ui.control.Label
        MassFlowN2EditField             matlab.ui.control.NumericEditField
        VolumetricFlowTotEditFieldLabel  matlab.ui.control.Label
        VolumetricFlowTotEditField      matlab.ui.control.NumericEditField
        MassFlowTotalEditFieldLabel     matlab.ui.control.Label
        MassFlowTotalEditField          matlab.ui.control.NumericEditField
        InletVelocityEditFieldLabel     matlab.ui.control.Label
        InletVelocityEditField          matlab.ui.control.NumericEditField
        ULabel_3                        matlab.ui.control.Label
        ULabel_4                        matlab.ui.control.Label
        ULabel_5                        matlab.ui.control.Label
        ULabel_6                        matlab.ui.control.Label
        msLabel                         matlab.ui.control.Label
        DataTranscriptionPanel          matlab.ui.container.Panel
        GridLayout9                     matlab.ui.container.GridLayout
        ChooseFolderButton              matlab.ui.control.Button
        FolderSelectedEditFieldLabel    matlab.ui.control.Label
        FolderSelectedEditField         matlab.ui.control.EditField
        DocumentTitleEditFieldLabel     matlab.ui.control.Label
        DocumentTitleEditField          matlab.ui.control.EditField
        SendLiveButton                  matlab.ui.control.Button
    end

    % Properties that correspond to apps with auto-reflow
    properties (Access = private)
        onePanelWidth = 576;
        twoPanelWidth = 768;
    end

    
    properties (Access = public)
        f % Description
        ar %
        running %
        addrs%
        adc %
        WireReady
        TubeReady
        FlowReady
        ScaleReady
        Tab
        AllData
        Sensors
        Sensor_report
        Pressure_running
    end
    
    methods (Access = public)
        
        function Ready1(app)
            if (app.WireReady == 1) && (app.TubeReady == 1) && ...
                    (app.FlowReady == 1) && (app.ScaleReady == 1)
                set(app.ChamberBackfillPanel,'visible','on')
            end
        end
        
        function Ready2(app)
            if (app.ChamberReady == 1)
                set(app.StartRunPanel,'visible','on')
            end
        end
    end
    

    % Callbacks that handle component events
    methods (Access = private)

        % Code that executes after component creation
        function startupFcn(app)
            clear;clc;
            
            app.addrs = 1;
        end

        % Size changed function: ChamberBackfillPanel
        function ChamberBackfillPanelSizeChanged(app, event)
            position = app.ChamberBackfillPanel.Position;
            
        end

        % Callback function
        function ReadyButtonPushed(app, event)
            set(app.ChamberBackfillPanel,'visible','on')
            set(app.StartRunPanel,'visible','on')
        end

        % Button pushed function: ChooseFolderButton
        function ChooseFolderButtonPushed(app, event)
            selpath =  uigetdir;
            app.FolderSelectedEditField.Value = selpath;
        end

        % Button pushed function: IgnitionButton
        function IgnitionButtonPushed(app, event)
            app.running = 1;
            % Clock start
            t = seconds(0);
            
            %Initial Readings here
            v1 = readVoltage(app.adc,0);
            %T = app.Sensors.Thermocouple.readHotJunc();
            %F = app.Sensors.Radiometer.Read();
            %W = app.Sensors.Scale.Read();            
            
            t_num = seconds(t);
            %Initiate Graphs/Tables=
            app.Tab = {t_num,v1};
            app.UITable.Data = [app.Tab];
            line = line(app.UIAxes,t,v1);
            %line1 = line(app.UIAxes,t,T);
            %line2 = line(app.UIAxes_2,t,F);
            %line3 = line(app.UIAxes_3,t,W);
            
            ylim(app.UIAxes,[3.41,3.43]);
            
            %Continuously runs, updates graphs/tables
            pause_len = .1;
            t(2) = t(1) + seconds(pause_len);
            i = 2;
            while (app.running)
                v1(i) = readVoltage(app.adc, 0);
                %T(i) = app.Sensors.Thermocouple.readHotJunc();
                %F(i) = app.Sensors.Radiometer.Read();
                %W(i) = app.Sensors.Scale.Read();    
                pause(pause_len)
                
                t_num(i) = seconds(t(i));
                line.XData = [line.XData t(i)];
                line.YData = [line.YData v1(i)];
                
                %line1.XData = [line1.XData t(i)];
                %line2.XData = [line2.XData t(i)];
                %line3.XData = [line3.XData t(i)];
                
                %line1.YData = [line.YData T(i)];
                %line2.YData = [line.YData F(i)];
                %line3.YData = [line.YData W(i)];
                
                app.Tab = [app.Tab;{t_num(i),v1(i)}];
                app.UITable.Data = [app.Tab];
                
                if t<seconds(60)
                    xlim(app.UIAxes,[seconds(0),seconds(60)])
                else
                    xlim(app.UIAxes,[t(i)-seconds(60),t(i)])
                end
                
                
                i = i+1;
                t(i) = t(i-1) + seconds(pause_len);
            end
            
        end

        % Button pushed function: RetractIgniterButton
        function RetractIgniterButtonPushed(app, event)
            app.running = 0;
            app.Transducers.ArmMotor.Run(); %Play with the inputs later Duration,DutyCycle
        end

        % Button pushed function: CheckButton
        function CheckButtonPushed(app, event)
            app.f = Func_Lib;
            app.ar = app.f.ConnectArduino();
            %[app.Sensors,app.Transducers] = app.f.Initialize(app.ar);
            %app.Sensor_report = app.f.SensorCheck();
            
            
            app.addrs = scanI2CBus(app.ar);
            app.adc = ads1115(app.ar, app.addrs{1});
            app.adc.VoltageScale = 4.096;
            
            app.FlowReady = 0;
            app.TubeReady = 0;
            app.ScaleReady = 0;
            app.WireReady = 1;
            
            app.Lamp.Color = 'green';
            
            
            
            Ready1(app);
            
            %Run livestream
            
        end

        % Button pushed function: CheckButton_2
        function CheckButton_2Pushed(app, event)
            app.TubeReady = 1;
            app.Lamp_2.Color = 'green';
            Ready1(app);
        end

        % Value changed function: O2Spinner
        function O2SpinnerValueChanged(app, event)
            value = app.O2Spinner.Value;
            if (value<=100) && (value>=0)
                app.N2Spinner.Value = 100-value;
            end
            
            app.FlowReady = 1;
            Ready1(app);
        end

        % Button pushed function: ResetScaleButton
        function ResetScaleButtonPushed(app, event)
            app.ScaleReady = 1;
            Ready1(app);
            
            
        end

        % Button pushed function: SendLiveButton
        function SendLiveButtonPushed(app, event)
            if ~isempty(app.DocumentTitleEditField.Value)
                fullname = append(app.FolderSelectedEditField.Value, app.DocumentTitleEditField.Value);
                
                xlswrite(fullname,app.AllData);
            end
        end

        % Button pushed function: EvacuateChamberButton
        function EvacuateChamberButtonPushed(app, event)
            app.Pressure_running = 1;
            while app.Pressure_running
                %{ 
                [P,T_P] = app.Sensors.Pressure.Read();
                
                if P < -Insert Vacuum Pressure-
                    app.Lamp_3.Color = 'green';
                end
                
                if P > -Insert Max Chamber Pressure Here-
                    app.Lamp_7.Color = 'red';
                end
                
                if T_P > -Insert Max Chamber Temp Here- Help prevent damage to sensors-
                    app.Lamp_8.Color = 'red';
                end
                
                app.ChamberPressureEditField.Value = P;
                app.SensorTemperatureEditField.Value = T_P;
                
                pause(.1)
                %}
            end
            
            
        end

        % Value changed function: PaperIgnitedCheckBox
        function PaperIgnitedCheckBoxValueChanged(app, event)
            value = app.PaperIgnitedCheckBox.Value;
            
        end

        % Changes arrangement of the app based on UIFigure width
        function updateAppLayout(app, event)
            currentFigureWidth = app.UIFigure.Position(3);
            if(currentFigureWidth <= app.onePanelWidth)
                % Change to a 3x1 grid
                app.GridLayout.RowHeight = {886, 886, 886};
                app.GridLayout.ColumnWidth = {'1x'};
                app.CenterPanel.Layout.Row = 1;
                app.CenterPanel.Layout.Column = 1;
                app.LeftPanel.Layout.Row = 2;
                app.LeftPanel.Layout.Column = 1;
                app.RightPanel.Layout.Row = 3;
                app.RightPanel.Layout.Column = 1;
            elseif (currentFigureWidth > app.onePanelWidth && currentFigureWidth <= app.twoPanelWidth)
                % Change to a 2x2 grid
                app.GridLayout.RowHeight = {886, 886};
                app.GridLayout.ColumnWidth = {'1x', '1x'};
                app.CenterPanel.Layout.Row = 1;
                app.CenterPanel.Layout.Column = [1,2];
                app.LeftPanel.Layout.Row = 2;
                app.LeftPanel.Layout.Column = 1;
                app.RightPanel.Layout.Row = 2;
                app.RightPanel.Layout.Column = 2;
            else
                % Change to a 1x3 grid
                app.GridLayout.RowHeight = {'1x'};
                app.GridLayout.ColumnWidth = {295, '1x', 297};
                app.LeftPanel.Layout.Row = 1;
                app.LeftPanel.Layout.Column = 1;
                app.CenterPanel.Layout.Row = 1;
                app.CenterPanel.Layout.Column = 2;
                app.RightPanel.Layout.Row = 1;
                app.RightPanel.Layout.Column = 3;
            end
        end
    end

    % Component initialization
    methods (Access = private)

        % Create UIFigure and components
        function createComponents(app)

            % Create UIFigure and hide until all components are created
            app.UIFigure = uifigure('Visible', 'off');
            app.UIFigure.AutoResizeChildren = 'off';
            app.UIFigure.Position = [100 100 1474 886];
            app.UIFigure.Name = 'UI Figure';
            app.UIFigure.SizeChangedFcn = createCallbackFcn(app, @updateAppLayout, true);

            % Create GridLayout
            app.GridLayout = uigridlayout(app.UIFigure);
            app.GridLayout.ColumnWidth = {295, '1x', 297};
            app.GridLayout.RowHeight = {'1x'};
            app.GridLayout.ColumnSpacing = 0;
            app.GridLayout.RowSpacing = 0;
            app.GridLayout.Padding = [0 0 0 0];
            app.GridLayout.Scrollable = 'on';

            % Create LeftPanel
            app.LeftPanel = uipanel(app.GridLayout);
            app.LeftPanel.Layout.Row = 1;
            app.LeftPanel.Layout.Column = 1;

            % Create SetupPanel
            app.SetupPanel = uipanel(app.LeftPanel);
            app.SetupPanel.TitlePosition = 'centertop';
            app.SetupPanel.Title = 'Setup';
            app.SetupPanel.FontWeight = 'bold';
            app.SetupPanel.Position = [11 177 270 698];

            % Create GridLayout2
            app.GridLayout2 = uigridlayout(app.SetupPanel);
            app.GridLayout2.RowHeight = {'fit', 'fit', 'fit', 'fit', 'fit'};

            % Create BasicChecksPanel
            app.BasicChecksPanel = uipanel(app.GridLayout2);
            app.BasicChecksPanel.TitlePosition = 'centertop';
            app.BasicChecksPanel.Title = 'Basic Checks';
            app.BasicChecksPanel.Layout.Row = 1;
            app.BasicChecksPanel.Layout.Column = [1 2];
            app.BasicChecksPanel.FontWeight = 'bold';

            % Create GridLayout3
            app.GridLayout3 = uigridlayout(app.BasicChecksPanel);
            app.GridLayout3.ColumnWidth = {'3x', 'fit'};
            app.GridLayout3.RowHeight = {'fit', '1x', '1x', 'fit', '1x', 'fit'};

            % Create CheckButton
            app.CheckButton = uibutton(app.GridLayout3, 'push');
            app.CheckButton.ButtonPushedFcn = createCallbackFcn(app, @CheckButtonPushed, true);
            app.CheckButton.Layout.Row = 2;
            app.CheckButton.Layout.Column = 1;
            app.CheckButton.Text = 'Check';

            % Create Lamp
            app.Lamp = uilamp(app.GridLayout3);
            app.Lamp.Layout.Row = 2;
            app.Lamp.Layout.Column = 2;
            app.Lamp.Color = [1 0.4118 0.1608];

            % Create EditField
            app.EditField = uieditfield(app.GridLayout3, 'text');
            app.EditField.BackgroundColor = [0 1 1];
            app.EditField.Layout.Row = 3;
            app.EditField.Layout.Column = [1 2];
            app.EditField.Value = 'Not Run';

            % Create WiringCheckLabel
            app.WiringCheckLabel = uilabel(app.GridLayout3);
            app.WiringCheckLabel.HorizontalAlignment = 'center';
            app.WiringCheckLabel.FontWeight = 'bold';
            app.WiringCheckLabel.Layout.Row = 1;
            app.WiringCheckLabel.Layout.Column = [1 2];
            app.WiringCheckLabel.Text = 'Wiring Check';

            % Create TubingCheckLabel
            app.TubingCheckLabel = uilabel(app.GridLayout3);
            app.TubingCheckLabel.HorizontalAlignment = 'center';
            app.TubingCheckLabel.FontWeight = 'bold';
            app.TubingCheckLabel.Layout.Row = 4;
            app.TubingCheckLabel.Layout.Column = [1 2];
            app.TubingCheckLabel.Text = 'Tubing Check';

            % Create CheckButton_2
            app.CheckButton_2 = uibutton(app.GridLayout3, 'push');
            app.CheckButton_2.ButtonPushedFcn = createCallbackFcn(app, @CheckButton_2Pushed, true);
            app.CheckButton_2.VerticalAlignment = 'top';
            app.CheckButton_2.Layout.Row = 5;
            app.CheckButton_2.Layout.Column = 1;
            app.CheckButton_2.Text = 'Check';

            % Create Lamp_2
            app.Lamp_2 = uilamp(app.GridLayout3);
            app.Lamp_2.Layout.Row = 5;
            app.Lamp_2.Layout.Column = 2;
            app.Lamp_2.Color = [1 0.4118 0.1608];

            % Create ManualCheckLabel
            app.ManualCheckLabel = uilabel(app.GridLayout3);
            app.ManualCheckLabel.HorizontalAlignment = 'center';
            app.ManualCheckLabel.FontSize = 11;
            app.ManualCheckLabel.Layout.Row = 6;
            app.ManualCheckLabel.Layout.Column = [1 2];
            app.ManualCheckLabel.Text = 'Manual Check';

            % Create FlowSettingsPanel
            app.FlowSettingsPanel = uipanel(app.GridLayout2);
            app.FlowSettingsPanel.TitlePosition = 'centertop';
            app.FlowSettingsPanel.Title = 'Flow Settings';
            app.FlowSettingsPanel.Layout.Row = 2;
            app.FlowSettingsPanel.Layout.Column = [1 2];
            app.FlowSettingsPanel.FontWeight = 'bold';

            % Create GridLayout7
            app.GridLayout7 = uigridlayout(app.FlowSettingsPanel);
            app.GridLayout7.ColumnWidth = {'fit', '1x'};

            % Create O2Label
            app.O2Label = uilabel(app.GridLayout7);
            app.O2Label.HorizontalAlignment = 'right';
            app.O2Label.Layout.Row = 1;
            app.O2Label.Layout.Column = 1;
            app.O2Label.Text = 'O2 %';

            % Create O2Spinner
            app.O2Spinner = uispinner(app.GridLayout7);
            app.O2Spinner.ValueChangedFcn = createCallbackFcn(app, @O2SpinnerValueChanged, true);
            app.O2Spinner.Layout.Row = 1;
            app.O2Spinner.Layout.Column = 2;

            % Create N2SpinnerLabel
            app.N2SpinnerLabel = uilabel(app.GridLayout7);
            app.N2SpinnerLabel.HorizontalAlignment = 'right';
            app.N2SpinnerLabel.Layout.Row = 2;
            app.N2SpinnerLabel.Layout.Column = 1;
            app.N2SpinnerLabel.Text = 'N2 %';

            % Create N2Spinner
            app.N2Spinner = uispinner(app.GridLayout7);
            app.N2Spinner.BackgroundColor = [0 1 1];
            app.N2Spinner.Layout.Row = 2;
            app.N2Spinner.Layout.Column = 2;

            % Create WeightScaleResetPanel
            app.WeightScaleResetPanel = uipanel(app.GridLayout2);
            app.WeightScaleResetPanel.TitlePosition = 'centertop';
            app.WeightScaleResetPanel.Title = 'Weight Scale Reset';
            app.WeightScaleResetPanel.Layout.Row = 3;
            app.WeightScaleResetPanel.Layout.Column = [1 2];
            app.WeightScaleResetPanel.FontWeight = 'bold';

            % Create GridLayout5
            app.GridLayout5 = uigridlayout(app.WeightScaleResetPanel);
            app.GridLayout5.ColumnWidth = {'1x', '1x', 'fit'};

            % Create ResetScaleButton
            app.ResetScaleButton = uibutton(app.GridLayout5, 'push');
            app.ResetScaleButton.ButtonPushedFcn = createCallbackFcn(app, @ResetScaleButtonPushed, true);
            app.ResetScaleButton.Layout.Row = 2;
            app.ResetScaleButton.Layout.Column = [1 2];
            app.ResetScaleButton.Text = 'Reset Scale';

            % Create InitialWeightLabel
            app.InitialWeightLabel = uilabel(app.GridLayout5);
            app.InitialWeightLabel.HorizontalAlignment = 'center';
            app.InitialWeightLabel.Layout.Row = 1;
            app.InitialWeightLabel.Layout.Column = 1;
            app.InitialWeightLabel.Text = 'Initial Weight';

            % Create gEditFieldLabel
            app.gEditFieldLabel = uilabel(app.GridLayout5);
            app.gEditFieldLabel.Layout.Row = 1;
            app.gEditFieldLabel.Layout.Column = 3;
            app.gEditFieldLabel.Text = 'g';

            % Create gEditField
            app.gEditField = uieditfield(app.GridLayout5, 'numeric');
            app.gEditField.Layout.Row = 1;
            app.gEditField.Layout.Column = 2;

            % Create ChamberBackfillPanel
            app.ChamberBackfillPanel = uipanel(app.GridLayout2);
            app.ChamberBackfillPanel.TitlePosition = 'centertop';
            app.ChamberBackfillPanel.Title = 'Chamber Backfill';
            app.ChamberBackfillPanel.SizeChangedFcn = createCallbackFcn(app, @ChamberBackfillPanelSizeChanged, true);
            app.ChamberBackfillPanel.Layout.Row = 4;
            app.ChamberBackfillPanel.Layout.Column = [1 2];
            app.ChamberBackfillPanel.FontWeight = 'bold';

            % Create GridLayout6
            app.GridLayout6 = uigridlayout(app.ChamberBackfillPanel);
            app.GridLayout6.ColumnWidth = {'1x', '1x', 'fit'};
            app.GridLayout6.RowHeight = {'1x', '1x', '1x'};

            % Create EvacuateChamberButton
            app.EvacuateChamberButton = uibutton(app.GridLayout6, 'push');
            app.EvacuateChamberButton.ButtonPushedFcn = createCallbackFcn(app, @EvacuateChamberButtonPushed, true);
            app.EvacuateChamberButton.Layout.Row = 1;
            app.EvacuateChamberButton.Layout.Column = [1 2];
            app.EvacuateChamberButton.Text = 'Evacuate Chamber';

            % Create kPaEditFieldLabel
            app.kPaEditFieldLabel = uilabel(app.GridLayout6);
            app.kPaEditFieldLabel.Layout.Row = 3;
            app.kPaEditFieldLabel.Layout.Column = 3;
            app.kPaEditFieldLabel.Text = 'kPa';

            % Create kPaEditField
            app.kPaEditField = uieditfield(app.GridLayout6, 'numeric');
            app.kPaEditField.BackgroundColor = [0 1 1];
            app.kPaEditField.Layout.Row = 3;
            app.kPaEditField.Layout.Column = 2;

            % Create PressureLabel
            app.PressureLabel = uilabel(app.GridLayout6);
            app.PressureLabel.HorizontalAlignment = 'center';
            app.PressureLabel.Layout.Row = 3;
            app.PressureLabel.Layout.Column = 1;
            app.PressureLabel.Text = 'Pressure:';

            % Create RefillChamberButton
            app.RefillChamberButton = uibutton(app.GridLayout6, 'push');
            app.RefillChamberButton.Layout.Row = 2;
            app.RefillChamberButton.Layout.Column = [1 2];
            app.RefillChamberButton.Text = 'Refill Chamber';

            % Create Lamp_3
            app.Lamp_3 = uilamp(app.GridLayout6);
            app.Lamp_3.Layout.Row = 1;
            app.Lamp_3.Layout.Column = 3;
            app.Lamp_3.Color = [1 0.4118 0.1608];

            % Create Lamp_4
            app.Lamp_4 = uilamp(app.GridLayout6);
            app.Lamp_4.Layout.Row = 2;
            app.Lamp_4.Layout.Column = 3;
            app.Lamp_4.Color = [1 0.4118 0.1608];

            % Create StartRunPanel
            app.StartRunPanel = uipanel(app.GridLayout2);
            app.StartRunPanel.TitlePosition = 'centertop';
            app.StartRunPanel.Title = 'Start Run';
            app.StartRunPanel.Layout.Row = 5;
            app.StartRunPanel.Layout.Column = [1 2];
            app.StartRunPanel.FontWeight = 'bold';

            % Create GridLayout8
            app.GridLayout8 = uigridlayout(app.StartRunPanel);
            app.GridLayout8.ColumnWidth = {'1x', '1x', 'fit'};

            % Create IgnitionButton
            app.IgnitionButton = uibutton(app.GridLayout8, 'push');
            app.IgnitionButton.ButtonPushedFcn = createCallbackFcn(app, @IgnitionButtonPushed, true);
            app.IgnitionButton.Layout.Row = 1;
            app.IgnitionButton.Layout.Column = [1 2];
            app.IgnitionButton.Text = 'Ignition';

            % Create Lamp_5
            app.Lamp_5 = uilamp(app.GridLayout8);
            app.Lamp_5.Layout.Row = 1;
            app.Lamp_5.Layout.Column = 3;
            app.Lamp_5.Color = [1 0.4118 0.1608];

            % Create RetractIgniterButton
            app.RetractIgniterButton = uibutton(app.GridLayout8, 'push');
            app.RetractIgniterButton.ButtonPushedFcn = createCallbackFcn(app, @RetractIgniterButtonPushed, true);
            app.RetractIgniterButton.Layout.Row = 2;
            app.RetractIgniterButton.Layout.Column = [1 2];
            app.RetractIgniterButton.Text = 'Retract Igniter';

            % Create Lamp_6
            app.Lamp_6 = uilamp(app.GridLayout8);
            app.Lamp_6.Layout.Row = 2;
            app.Lamp_6.Layout.Column = 3;
            app.Lamp_6.Color = [1 0.4118 0.1608];

            % Create CenterPanel
            app.CenterPanel = uipanel(app.GridLayout);
            app.CenterPanel.Layout.Row = 1;
            app.CenterPanel.Layout.Column = 2;

            % Create LivestreamPanel
            app.LivestreamPanel = uipanel(app.CenterPanel);
            app.LivestreamPanel.TitlePosition = 'centertop';
            app.LivestreamPanel.Title = 'Livestream';
            app.LivestreamPanel.FontWeight = 'bold';
            app.LivestreamPanel.Position = [161 525 560 330];

            % Create TabGroup
            app.TabGroup = uitabgroup(app.CenterPanel);
            app.TabGroup.Position = [31 34 810 441];

            % Create LiveGraphsTab
            app.LiveGraphsTab = uitab(app.TabGroup);
            app.LiveGraphsTab.Title = 'Live Graphs';

            % Create GridLayout14
            app.GridLayout14 = uigridlayout(app.LiveGraphsTab);
            app.GridLayout14.ColumnWidth = {'1x'};
            app.GridLayout14.RowHeight = {'1x'};

            % Create TabGroup2
            app.TabGroup2 = uitabgroup(app.GridLayout14);
            app.TabGroup2.Layout.Row = 1;
            app.TabGroup2.Layout.Column = 1;

            % Create TemperatureTab
            app.TemperatureTab = uitab(app.TabGroup2);
            app.TemperatureTab.Title = 'Temperature';

            % Create GridLayout15
            app.GridLayout15 = uigridlayout(app.TemperatureTab);
            app.GridLayout15.ColumnWidth = {'1x'};
            app.GridLayout15.RowHeight = {'1x'};

            % Create UIAxes
            app.UIAxes = uiaxes(app.GridLayout15);
            title(app.UIAxes, 'Temperature vs Time')
            xlabel(app.UIAxes, 't')
            ylabel(app.UIAxes, 'T(C)')
            app.UIAxes.PlotBoxAspectRatio = [2.44557823129252 1 1];
            app.UIAxes.Layout.Row = 1;
            app.UIAxes.Layout.Column = 1;

            % Create RadiativeFluxTab
            app.RadiativeFluxTab = uitab(app.TabGroup2);
            app.RadiativeFluxTab.Title = 'Radiative Flux';

            % Create GridLayout15_2
            app.GridLayout15_2 = uigridlayout(app.RadiativeFluxTab);
            app.GridLayout15_2.ColumnWidth = {'1x'};
            app.GridLayout15_2.RowHeight = {'1x'};

            % Create UIAxes_2
            app.UIAxes_2 = uiaxes(app.GridLayout15_2);
            title(app.UIAxes_2, 'Radiative Flux vs Time')
            xlabel(app.UIAxes_2, 't')
            ylabel(app.UIAxes_2, 'Radiaitive Flux(mW/cm^2)')
            app.UIAxes_2.PlotBoxAspectRatio = [2.42372881355932 1 1];
            app.UIAxes_2.Layout.Row = 1;
            app.UIAxes_2.Layout.Column = 1;

            % Create ChangeinWeightTab
            app.ChangeinWeightTab = uitab(app.TabGroup2);
            app.ChangeinWeightTab.Title = 'Change in Weight';

            % Create GridLayout15_3
            app.GridLayout15_3 = uigridlayout(app.ChangeinWeightTab);
            app.GridLayout15_3.ColumnWidth = {'1x'};
            app.GridLayout15_3.RowHeight = {'1x'};

            % Create UIAxes2
            app.UIAxes2 = uiaxes(app.GridLayout15_3);
            title(app.UIAxes2, 'Change in Weight vs Time')
            xlabel(app.UIAxes2, 't')
            ylabel(app.UIAxes2, 'Change in Weight(g)')
            app.UIAxes2.PlotBoxAspectRatio = [2.44557823129252 1 1];
            app.UIAxes2.Layout.Row = 1;
            app.UIAxes2.Layout.Column = 1;

            % Create LiveTables
            app.LiveTables = uitab(app.TabGroup);
            app.LiveTables.Title = 'Live Tables';

            % Create GridLayout16
            app.GridLayout16 = uigridlayout(app.LiveTables);
            app.GridLayout16.ColumnWidth = {'1x'};
            app.GridLayout16.RowHeight = {'1x'};

            % Create UITable
            app.UITable = uitable(app.GridLayout16);
            app.UITable.ColumnName = {'Time'; 'Temperature'; 'Radiative Flux'; 'Change in Weight'};
            app.UITable.RowName = {};
            app.UITable.Layout.Row = 1;
            app.UITable.Layout.Column = 1;

            % Create RightPanel
            app.RightPanel = uipanel(app.GridLayout);
            app.RightPanel.Layout.Row = 1;
            app.RightPanel.Layout.Column = 3;

            % Create VitalsWarningsPanel
            app.VitalsWarningsPanel = uipanel(app.RightPanel);
            app.VitalsWarningsPanel.TitlePosition = 'centertop';
            app.VitalsWarningsPanel.Title = 'Vitals??/Warnings';
            app.VitalsWarningsPanel.FontWeight = 'bold';
            app.VitalsWarningsPanel.Position = [11 405 270 470];

            % Create GridLayout10
            app.GridLayout10 = uigridlayout(app.VitalsWarningsPanel);
            app.GridLayout10.RowHeight = {'fit', 'fit', 'fit'};

            % Create PressureSensorPanel
            app.PressureSensorPanel = uipanel(app.GridLayout10);
            app.PressureSensorPanel.TitlePosition = 'centertop';
            app.PressureSensorPanel.Title = 'Pressure Sensor';
            app.PressureSensorPanel.Layout.Row = 2;
            app.PressureSensorPanel.Layout.Column = [1 2];
            app.PressureSensorPanel.FontWeight = 'bold';

            % Create GridLayout11
            app.GridLayout11 = uigridlayout(app.PressureSensorPanel);
            app.GridLayout11.ColumnWidth = {'fit', '1x', 'fit', 'fit'};

            % Create ChamberPressureEditFieldLabel
            app.ChamberPressureEditFieldLabel = uilabel(app.GridLayout11);
            app.ChamberPressureEditFieldLabel.HorizontalAlignment = 'right';
            app.ChamberPressureEditFieldLabel.Layout.Row = 1;
            app.ChamberPressureEditFieldLabel.Layout.Column = 1;
            app.ChamberPressureEditFieldLabel.Text = 'Chamber Pressure';

            % Create ChamberPressureEditField
            app.ChamberPressureEditField = uieditfield(app.GridLayout11, 'numeric');
            app.ChamberPressureEditField.BackgroundColor = [0 1 1];
            app.ChamberPressureEditField.Layout.Row = 1;
            app.ChamberPressureEditField.Layout.Column = 2;

            % Create kPaLabel
            app.kPaLabel = uilabel(app.GridLayout11);
            app.kPaLabel.Layout.Row = 1;
            app.kPaLabel.Layout.Column = 3;
            app.kPaLabel.Text = 'kPa';

            % Create Lamp_7
            app.Lamp_7 = uilamp(app.GridLayout11);
            app.Lamp_7.Layout.Row = 1;
            app.Lamp_7.Layout.Column = 4;

            % Create SensorTemperatureEditFieldLabel
            app.SensorTemperatureEditFieldLabel = uilabel(app.GridLayout11);
            app.SensorTemperatureEditFieldLabel.HorizontalAlignment = 'right';
            app.SensorTemperatureEditFieldLabel.Layout.Row = 2;
            app.SensorTemperatureEditFieldLabel.Layout.Column = 1;
            app.SensorTemperatureEditFieldLabel.Text = 'Sensor Temperature';

            % Create SensorTemperatureEditField
            app.SensorTemperatureEditField = uieditfield(app.GridLayout11, 'numeric');
            app.SensorTemperatureEditField.BackgroundColor = [0 1 1];
            app.SensorTemperatureEditField.Layout.Row = 2;
            app.SensorTemperatureEditField.Layout.Column = 2;

            % Create KLabel
            app.KLabel = uilabel(app.GridLayout11);
            app.KLabel.Layout.Row = 2;
            app.KLabel.Layout.Column = 3;
            app.KLabel.Text = 'K';

            % Create Lamp_8
            app.Lamp_8 = uilamp(app.GridLayout11);
            app.Lamp_8.Layout.Row = 2;
            app.Lamp_8.Layout.Column = 4;

            % Create PaperIgnitedPanel
            app.PaperIgnitedPanel = uipanel(app.GridLayout10);
            app.PaperIgnitedPanel.TitlePosition = 'centertop';
            app.PaperIgnitedPanel.Title = 'Paper Ignited';
            app.PaperIgnitedPanel.Layout.Row = 3;
            app.PaperIgnitedPanel.Layout.Column = [1 2];
            app.PaperIgnitedPanel.FontWeight = 'bold';

            % Create GridLayout12
            app.GridLayout12 = uigridlayout(app.PaperIgnitedPanel);
            app.GridLayout12.ColumnWidth = {'1x'};
            app.GridLayout12.RowHeight = {'fit'};

            % Create PaperIgnitedCheckBox
            app.PaperIgnitedCheckBox = uicheckbox(app.GridLayout12);
            app.PaperIgnitedCheckBox.ValueChangedFcn = createCallbackFcn(app, @PaperIgnitedCheckBoxValueChanged, true);
            app.PaperIgnitedCheckBox.Text = 'Paper Ignited';
            app.PaperIgnitedCheckBox.Layout.Row = 1;
            app.PaperIgnitedCheckBox.Layout.Column = 1;

            % Create MassFlowControllersPanel
            app.MassFlowControllersPanel = uipanel(app.GridLayout10);
            app.MassFlowControllersPanel.TitlePosition = 'centertop';
            app.MassFlowControllersPanel.Title = 'Mass Flow Controllers';
            app.MassFlowControllersPanel.Layout.Row = 1;
            app.MassFlowControllersPanel.Layout.Column = [1 2];
            app.MassFlowControllersPanel.FontWeight = 'bold';

            % Create GridLayout13
            app.GridLayout13 = uigridlayout(app.MassFlowControllersPanel);
            app.GridLayout13.ColumnWidth = {'fit', '1x', 'fit'};
            app.GridLayout13.RowHeight = {'1x', '1x', '1x', '1x', '1x', '1x', '1x'};

            % Create VolumetricFlowO2EditFieldLabel
            app.VolumetricFlowO2EditFieldLabel = uilabel(app.GridLayout13);
            app.VolumetricFlowO2EditFieldLabel.HorizontalAlignment = 'right';
            app.VolumetricFlowO2EditFieldLabel.Layout.Row = 1;
            app.VolumetricFlowO2EditFieldLabel.Layout.Column = 1;
            app.VolumetricFlowO2EditFieldLabel.Text = 'Volumetric Flow O2';

            % Create VolumetricFlowO2EditField
            app.VolumetricFlowO2EditField = uieditfield(app.GridLayout13, 'numeric');
            app.VolumetricFlowO2EditField.BackgroundColor = [0 1 1];
            app.VolumetricFlowO2EditField.Layout.Row = 1;
            app.VolumetricFlowO2EditField.Layout.Column = 2;

            % Create ULabel
            app.ULabel = uilabel(app.GridLayout13);
            app.ULabel.BackgroundColor = [1 0 0];
            app.ULabel.Layout.Row = 1;
            app.ULabel.Layout.Column = 3;
            app.ULabel.Text = 'U';

            % Create VolumetricFlowN2EditFieldLabel
            app.VolumetricFlowN2EditFieldLabel = uilabel(app.GridLayout13);
            app.VolumetricFlowN2EditFieldLabel.HorizontalAlignment = 'right';
            app.VolumetricFlowN2EditFieldLabel.Layout.Row = 2;
            app.VolumetricFlowN2EditFieldLabel.Layout.Column = 1;
            app.VolumetricFlowN2EditFieldLabel.Text = 'Volumetric Flow N2';

            % Create VolumetricFlowN2EditField
            app.VolumetricFlowN2EditField = uieditfield(app.GridLayout13, 'numeric');
            app.VolumetricFlowN2EditField.BackgroundColor = [0 1 1];
            app.VolumetricFlowN2EditField.Layout.Row = 2;
            app.VolumetricFlowN2EditField.Layout.Column = 2;

            % Create ULabel_2
            app.ULabel_2 = uilabel(app.GridLayout13);
            app.ULabel_2.BackgroundColor = [1 0 0];
            app.ULabel_2.Layout.Row = 2;
            app.ULabel_2.Layout.Column = 3;
            app.ULabel_2.Text = 'U';

            % Create MassFlowO2EditFieldLabel
            app.MassFlowO2EditFieldLabel = uilabel(app.GridLayout13);
            app.MassFlowO2EditFieldLabel.HorizontalAlignment = 'right';
            app.MassFlowO2EditFieldLabel.Layout.Row = 4;
            app.MassFlowO2EditFieldLabel.Layout.Column = 1;
            app.MassFlowO2EditFieldLabel.Text = 'Mass Flow O2';

            % Create MassFlowO2EditField
            app.MassFlowO2EditField = uieditfield(app.GridLayout13, 'numeric');
            app.MassFlowO2EditField.BackgroundColor = [0 1 1];
            app.MassFlowO2EditField.Layout.Row = 4;
            app.MassFlowO2EditField.Layout.Column = 2;

            % Create MassFlowN2EditFieldLabel
            app.MassFlowN2EditFieldLabel = uilabel(app.GridLayout13);
            app.MassFlowN2EditFieldLabel.HorizontalAlignment = 'right';
            app.MassFlowN2EditFieldLabel.Layout.Row = 5;
            app.MassFlowN2EditFieldLabel.Layout.Column = 1;
            app.MassFlowN2EditFieldLabel.Text = 'Mass Flow N2';

            % Create MassFlowN2EditField
            app.MassFlowN2EditField = uieditfield(app.GridLayout13, 'numeric');
            app.MassFlowN2EditField.BackgroundColor = [0 1 1];
            app.MassFlowN2EditField.Layout.Row = 5;
            app.MassFlowN2EditField.Layout.Column = 2;

            % Create VolumetricFlowTotEditFieldLabel
            app.VolumetricFlowTotEditFieldLabel = uilabel(app.GridLayout13);
            app.VolumetricFlowTotEditFieldLabel.HorizontalAlignment = 'right';
            app.VolumetricFlowTotEditFieldLabel.Layout.Row = 3;
            app.VolumetricFlowTotEditFieldLabel.Layout.Column = 1;
            app.VolumetricFlowTotEditFieldLabel.Text = 'Volumetric Flow Tot.';

            % Create VolumetricFlowTotEditField
            app.VolumetricFlowTotEditField = uieditfield(app.GridLayout13, 'numeric');
            app.VolumetricFlowTotEditField.BackgroundColor = [0 1 1];
            app.VolumetricFlowTotEditField.Layout.Row = 3;
            app.VolumetricFlowTotEditField.Layout.Column = 2;

            % Create MassFlowTotalEditFieldLabel
            app.MassFlowTotalEditFieldLabel = uilabel(app.GridLayout13);
            app.MassFlowTotalEditFieldLabel.HorizontalAlignment = 'right';
            app.MassFlowTotalEditFieldLabel.Layout.Row = 6;
            app.MassFlowTotalEditFieldLabel.Layout.Column = 1;
            app.MassFlowTotalEditFieldLabel.Text = 'Mass Flow Total';

            % Create MassFlowTotalEditField
            app.MassFlowTotalEditField = uieditfield(app.GridLayout13, 'numeric');
            app.MassFlowTotalEditField.BackgroundColor = [0 1 1];
            app.MassFlowTotalEditField.Layout.Row = 6;
            app.MassFlowTotalEditField.Layout.Column = 2;

            % Create InletVelocityEditFieldLabel
            app.InletVelocityEditFieldLabel = uilabel(app.GridLayout13);
            app.InletVelocityEditFieldLabel.HorizontalAlignment = 'right';
            app.InletVelocityEditFieldLabel.Layout.Row = 7;
            app.InletVelocityEditFieldLabel.Layout.Column = 1;
            app.InletVelocityEditFieldLabel.Text = 'Inlet Velocity';

            % Create InletVelocityEditField
            app.InletVelocityEditField = uieditfield(app.GridLayout13, 'numeric');
            app.InletVelocityEditField.BackgroundColor = [0 1 1];
            app.InletVelocityEditField.Layout.Row = 7;
            app.InletVelocityEditField.Layout.Column = 2;

            % Create ULabel_3
            app.ULabel_3 = uilabel(app.GridLayout13);
            app.ULabel_3.BackgroundColor = [1 0 0];
            app.ULabel_3.Layout.Row = 3;
            app.ULabel_3.Layout.Column = 3;
            app.ULabel_3.Text = 'U';

            % Create ULabel_4
            app.ULabel_4 = uilabel(app.GridLayout13);
            app.ULabel_4.BackgroundColor = [1 0 0];
            app.ULabel_4.Layout.Row = 4;
            app.ULabel_4.Layout.Column = 3;
            app.ULabel_4.Text = 'U';

            % Create ULabel_5
            app.ULabel_5 = uilabel(app.GridLayout13);
            app.ULabel_5.BackgroundColor = [1 0 0];
            app.ULabel_5.Layout.Row = 5;
            app.ULabel_5.Layout.Column = 3;
            app.ULabel_5.Text = 'U';

            % Create ULabel_6
            app.ULabel_6 = uilabel(app.GridLayout13);
            app.ULabel_6.BackgroundColor = [1 0 0];
            app.ULabel_6.Layout.Row = 6;
            app.ULabel_6.Layout.Column = 3;
            app.ULabel_6.Text = 'U';

            % Create msLabel
            app.msLabel = uilabel(app.GridLayout13);
            app.msLabel.Layout.Row = 7;
            app.msLabel.Layout.Column = 3;
            app.msLabel.Text = 'm/s';

            % Create DataTranscriptionPanel
            app.DataTranscriptionPanel = uipanel(app.RightPanel);
            app.DataTranscriptionPanel.TitlePosition = 'centertop';
            app.DataTranscriptionPanel.Title = 'Data Transcription';
            app.DataTranscriptionPanel.FontWeight = 'bold';
            app.DataTranscriptionPanel.Position = [11 232 270 163];

            % Create GridLayout9
            app.GridLayout9 = uigridlayout(app.DataTranscriptionPanel);
            app.GridLayout9.ColumnWidth = {'fit', '1x'};
            app.GridLayout9.RowHeight = {'fit', 'fit', 'fit', 'fit'};

            % Create ChooseFolderButton
            app.ChooseFolderButton = uibutton(app.GridLayout9, 'push');
            app.ChooseFolderButton.ButtonPushedFcn = createCallbackFcn(app, @ChooseFolderButtonPushed, true);
            app.ChooseFolderButton.Layout.Row = 1;
            app.ChooseFolderButton.Layout.Column = [1 2];
            app.ChooseFolderButton.Text = 'Choose Folder';

            % Create FolderSelectedEditFieldLabel
            app.FolderSelectedEditFieldLabel = uilabel(app.GridLayout9);
            app.FolderSelectedEditFieldLabel.HorizontalAlignment = 'right';
            app.FolderSelectedEditFieldLabel.Layout.Row = 2;
            app.FolderSelectedEditFieldLabel.Layout.Column = 1;
            app.FolderSelectedEditFieldLabel.Text = 'Folder Selected:';

            % Create FolderSelectedEditField
            app.FolderSelectedEditField = uieditfield(app.GridLayout9, 'text');
            app.FolderSelectedEditField.BackgroundColor = [0 1 1];
            app.FolderSelectedEditField.Layout.Row = 2;
            app.FolderSelectedEditField.Layout.Column = 2;

            % Create DocumentTitleEditFieldLabel
            app.DocumentTitleEditFieldLabel = uilabel(app.GridLayout9);
            app.DocumentTitleEditFieldLabel.HorizontalAlignment = 'right';
            app.DocumentTitleEditFieldLabel.Layout.Row = 3;
            app.DocumentTitleEditFieldLabel.Layout.Column = 1;
            app.DocumentTitleEditFieldLabel.Text = 'Document Title:';

            % Create DocumentTitleEditField
            app.DocumentTitleEditField = uieditfield(app.GridLayout9, 'text');
            app.DocumentTitleEditField.Layout.Row = 3;
            app.DocumentTitleEditField.Layout.Column = 2;

            % Create SendLiveButton
            app.SendLiveButton = uibutton(app.GridLayout9, 'push');
            app.SendLiveButton.ButtonPushedFcn = createCallbackFcn(app, @SendLiveButtonPushed, true);
            app.SendLiveButton.Layout.Row = 4;
            app.SendLiveButton.Layout.Column = [1 2];
            app.SendLiveButton.Text = 'Send Live';

            % Show the figure after all components are created
            app.UIFigure.Visible = 'on';
        end
    end

    % App creation and deletion
    methods (Access = public)

        % Construct app
        function app = NASA_Test_1_exported

            % Create UIFigure and components
            createComponents(app)

            % Register the app with App Designer
            registerApp(app, app.UIFigure)

            % Execute the startup function
            runStartupFcn(app, @startupFcn)

            if nargout == 0
                clear app
            end
        end

        % Code that executes before app deletion
        function delete(app)

            % Delete UIFigure when app is deleted
            delete(app.UIFigure)
        end
    end
end