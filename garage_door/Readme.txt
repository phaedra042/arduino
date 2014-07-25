======
placeholder for garage door controller with rfid control

output 1 door switch
output 2 buzzer
output 3 alarm

output 4,5,6 staus led (used for rfid reader)

door_open_delay =0 #what is counter for? primarily to see if the user has pressed the "keep the door open" button long enough 
door state = 0 # definition of door state? 0, 1, 2,3 ? ..open,closed,no state swtches operated,both state switches operated
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
	{alarm state off
	check state of door
    		if double input increase door_open_delay. double input is both open and closed switches operated
            else door_open_delay = 0
        timer tick() #I am mighty - what time interval is tick? as yet to be decided
        timer--
        	if door_open_delay = 10 then increase timer by 100(real numbers later) and  beep
            if timer= 10 then beep
           } 
if timer = 0 operate door motor else noop
}

known state closed{
check door state
while door closed
{
alarm state off
check state of door
try to read rfid
if authenticates, operate door and wait a bit
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

if transition timer=0 alarm state on
}


main loop{
known state closed()
unknown state() # if neither switch is operated i can't say definitivly what's happening.. therefor unknown.which means wait and alarm if needed
known state open()
}