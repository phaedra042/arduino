======
placeholder for garage door controller with rfid control

output 1 door switch
output 2 buzzer
output 3 alarm

output 4,5,6 staus led (used for rfid reader)

counter =0 #what is counter for? primarily to see if the user has pressed the "keep the door open" button long enough 
door state = 0 # definition of door state? 0, 1, 2,3 ? ..open,closed,no state swtches operated,both state switches operated
// door state suggestion 0=no sw closed, 1=1 switch closed say "closed door" closed, 2="opened door" switch closed, 3=both switches closed, then if that variable is tied to the inputs D0-D2 the byte automatically is the required value.
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
    		if double input increase counter. double input is both open and closed switches operated
            else counter = 0
        timer tick() #I am mighty - what time interval is tick? as yet to be decided - what does tick() function do?
        timer--
        	if counter = 10 then increase timer by 100(real numbers later) and  beep
            if timer= 10 then beep # suggest beep for each value of timer between 10 and 0
           } 
if timer = 0 operate door motor else noop # insert delay of transition time
}

known state closed{
check door state
while door closed
{
check state of door
try to read rfid
if authenticates, operate door and wait a bit [transition time]
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
unknown state() # if neither switch is operated i can't say definitivly what's happening.. therefore unknown.which means wait and alarm if needed
known state open()
}
