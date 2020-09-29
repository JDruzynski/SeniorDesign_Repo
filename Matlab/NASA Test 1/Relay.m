classdef Relay < handle & matlab.mixin.CustomDisplay
    %Relay class for easy use of Qwiic single relay
    %{   
    The constructor sets up the I2C device object. The standard address is 0x18, but can be set to 0x19.
 
    The Switch method simply writes to the device to open or close the
    relay. 
    
    It is advised to have high power circuit connected to NO and COM
    ports.This will let the circuit be open until told to switch.
    %}
    properties
        Address = hex2dec('0x18');
        Dev;
    end
    
    methods
        function obj = Relay(arduino,address)
            if nargin > 1
                obj.Address = address;
            end
            
            obj.Dev = i2cdev(arduino, obj.Address);
        end
        
        function Switch(obj)
            write(obj.Dev,hex2dec('0x00'),'uint16')
        end
    end
    
    methods
        %This simply allows it to take hex or numeric input
        function set.Address(obj, value)
            if isnumeric(value)
                validateattributes(value, {'numeric'}, ...
                    {'scalar', 'nonnegative'}, '', 'Address');
            else
                validateattributes(value, {'char'}, ...
                    {'nonempty'}, '', 'Address');
                value = obj.hex2dec(value);
            end
            if (value < obj.hex2dec('0x48')) || (value > obj.hex2dec('0x51'))
                error('raspi:ads1115:InvalidI2CAddress', ...
                    'Invalid I2C address. I2C address must be one of the following: 0x48, 0x49, 0x50, 0x51');
            end
            obj.Address = value;
        end
    end
end

