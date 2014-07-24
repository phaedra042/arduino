======
placeholder for garage door controller with rfid control

output 1 door switch
output 2 buzzer
output 3 alarm

output 4,5,6 staus led (used for rfid reader)

counter =0 #what is counter for?
door state = 0 # definition of door state? 0, 1, 2 ? 
timer= 100
transition time=100

init (){
set up 3 outputs and 2 switch inputs
set up status light
set up rfid reader
}

known state open{
timer=100
check state of door
if door open single beep
while door open and timer>0
	{check state of door
    		if double input increase counter #I presume that double input means neither open nor closed. Existing switches are low when open or closed.
            else counter = 0
        timer tick() #I am mighty - what time interval is tick?
        timer--
        	if counter = 10 then increase timer by 100(real numbers later)
            if timer= 10 then beep
           } 
if timer = 0 toggle door switch else noop #does toggle door switch mean "operate door motor"?
}

known state closed{
check door state
while door closed
{
check state of door
try to read rfid
if authenticates, toggle door switch and wait a bit
timer tick()
}

}

unknown state{
double beep
transition time=100
{
check state of door
timer tick ()
transition time--
}while door state unknown(or double input) or transition timer >0

if transition timer=0 alarm
}


main loop{
known state closed()
unknown state() # should not have an unknown state - if not open or closed, then it is transitioning or stopped between open or closed.
known state open()
}
