#!/bin/bash

start ()
{
	back_server=`pidof myserver | wc -w`
	front_UI=`pidof myUI | wc -w`
	if [ $back_server -eq 0 ]; then
		if [ -f myserver ]; then	
			./myserver 
			printf "myserver service start                 [\033[32m Success \033[0m]\n"
		else
			printf "myserver application not exit  [\033[31m Failure \033[0m]\n"
		fi
	else
		printf "myserver has been running              [\033[31m Failure \033[0m]\n"
	fi
	
	if [ $front_UI -eq 0 ]; then
		if [ -f myUI ]; then
			./myUI 
			sleep 4
			printf "myUI service start                 [\033[32m Success \033[0m]\n"
		else
			printf "myUI application not exit  [\033[31m Failure \033[0m]\n"
		fi
	else
		printf "myUI has been running              [\033[31m Failure \033[0m]\n"
	fi
}

stop ()
{
	back_server=`pidof myserver | wc -w`
	front_UI=`pidof myUI | wc -w`
	if [ $back_server -eq 0 ]; then
		printf "myserver not hava process		  [\033[31m Failure \033[0m]\n"
	else
		for curpid in `pidof myserver`; do
			#kill -12 $curpid	
			kill -9 $curpid
		done
		printf "myserver stop			  [\033[32m Success \033[0m]\n"
	fi	
	
	if [ $front_UI -eq 0 ]; then
		printf "myUI not hava process		  [\033[31m Failure \033[0m]\n"
	else
		for curpid in `pidof myUI`; do
			#kill -12 $curpid	
			kill -9 $curpid
		done
		printf "myUI stop			  [\033[32m Success \033[0m]\n"
	fi	
	
	./clear
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
