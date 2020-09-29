classdef DCMotor < handle & matlab.mixin.CustomDisplay
    %DC Motor class for easy use of L293D motor Controller and DC Motor
    %{   
    Preset to run at 100% duty cycle for 1 minute, with pin D5 of Arduino connected to
    the enable 1 pin of L293D, pin D3 connected to out1 of L293D, and pin
    D4 connected to out 2  of L293D. 
    
    To set different pins, simply put them in constructor. Can take 1-4
    arguments.
    
    To run motor, call Run method. Without input aruments it will run at
    100% duty cycle for 1 minute. If you want to change the duration or
    duty cycle, simply input when calling method. Note, if you want to
    input a duty cycle, you need to input a duration as well.
    
    Note - Currently only runs one direction. Currently no need to make it
    go the other way, will code it in if needed.
    %}
    properties
        Arduino
        Enable  = 'D5';
        DIR_A = 'D3';
        DIR_B = 'D4';
        Duration = 1;
        DutyCycle = 1;
        
    end
    
    methods
        function obj = DCMotor(arduinoObj,enable_pin,dir_a_pin,dir_b_pin)
            %UNTITLED Construct an instance of this class
            %   Detailed explanation goes here
            switch nargin
                case 1
                    obj.Arduino = arduinoObj;
                case 2
                    obj.Arduino = arduinoObj;
                    obj.Enable = enable_pin;
                case 3
                    obj.Arduino = arduinoObj;
                    obj.Enable = enable_pin;
                    obj.DIR_A = dir_a_pin;
                case 4
                    obj.Arduino = arduinoObj;
                    obj.Enable = enable_pin;
                    obj.DIR_A = dir_a_pin;
                    obj.DIR_B = dir_b_pin;
            end
        end
        
        function Run(obj,Duration,DutyCycle)
            switch nargin
                case 2
                    obj.Duration = Duration;
                case 3
                    obj.Duration = Duration;
                    obj.DutyCycle = DutyCycle;
            end
            
            writePWMDutyCycle(obj.Arduino,enable,obj.DutyCycle)
            writeDigitalPin(obj.Arduino,obj.DIR_A,1);
            pause(obj.Duration)
            writeDigitalPin(obj.Arduino,obj.DIR_A,0);
        end
    end
end

