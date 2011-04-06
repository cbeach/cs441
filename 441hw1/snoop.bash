#Casey Beach Copyright 2011
#snoop.bash
#
#
#

USERS=	$(who | egrep -v "cbeach|root" | wc -l)
HOUR=	$(date | egrep -o [0-9]{2}:[0-9]{2}:[0-9]{2} | cut -c 1-2)
HOST=	$(hostname)

while [ $HOUR -lt $1 ] 
do
	USERS=$(who | egrep -v "cbeach|root" | wc -l)
	HOUR=$(date | egrep -o [0-9]{2}:[0-9]{2}:[0-9]{2} | cut -c 1-2)

	if [ $USERS -gt 0 ]; then
		echo sleeping...
		sleep 60

		else
			echo wheeee!
	fi
done
echo finished
