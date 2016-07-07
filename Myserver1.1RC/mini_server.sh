#!/bin/bash

start ()
{
	server_run=`pidof server | wc -w`
		if [ $server_run -eq 0 ]; then
		if [ -f server ]; then
			rm -f server.txt			
			./server 
			printf "server service start                 [\033[32m Success \033[0m]\n"
		else
			printf "server application not exit  [\033[31m Failure \033[0m]\n"
		fi
	else
		printf "server has been running              [\033[31m Failure \033[0m]\n"
	fi
	
	
}

stop ()
{
	server_run=`pidof server | wc -w`
	
	if [ $server_run -eq 0 ]; then
		printf "server not have process		  [\033[31m Failure \033[0m]\n"
	else
		for curpid in `pidof server`; do
			#kill -12 $curpid	
			kill -9 $curpid
		done
		printf "server stop			  [\033[32m Success \033[0m]\n"
	fi	
	
		
	
	}
if [ $# -ne 1 ]
then
	echo ÓÃ·¨:./mini_server.sh start/stop/restart
else
	case $1 in
	start)
		start
		;;
	stop)
		stop
		;;
	restart)
		stop
		start
		;;
	esac
fi
exit
