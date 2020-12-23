#!/bin/bash
# You are NOT allowed to change the files' names!
domainNames="domainNames.txt"
IPAddresses="IPAddresses.txt"
adblockRules="adblockRules"
# create a regular expression for catching ipv6 from host response
ipv6_reg="([a-f0-9:]+:+)+[a-f0-9]+"
# create a regular expression for catching ipv4 from host response
ipv4_reg="[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}"

function adBlock() {
    if [ "$EUID" -ne 0 ];then
        printf "Please run as root.\n"
        exit 1
    fi
    if [ "$1" = "-domains"  ]; then
        while IFS= read -r line
        do
            (host $line | egrep -o "$ipv4_reg|$ipv6_reg") &
            sleep 0.3
        done < "$domainNames" > $IPAddresses
        # wait for background processes to finish before continue to the next commands
        wait
        # adding ipv4 address from IPAddresses file in iptables
        egrep -o "$ipv4_reg" $IPAddresses | while read -r line
        do
            iptables -A OUTPUT -d $line -j REJECT
        done
        # adding ipv6 address from IPAddresses file in ip6tables
        egrep -o "$ipv6_reg" $IPAddresses | while read -r line
        do
            ip6tables -A OUTPUT -d $line -j REJECT
        done
        true
        
    elif [ "$1" = "-ips"  ]; then
    # adding ipv4 address from IPAddresses file in iptables
        egrep -o "$ipv4_reg" $IPAddresses | while read -r line
        do
            iptables -A OUTPUT -d $line -j REJECT
        done
        # adding ipv6 address from IPAddresses file in ip6tables
        egrep -o "$ipv6_reg" $IPAddresses | while read -r line
        do
            ip6tables -A OUTPUT -d $line -j REJECT
        done
        true
        
    elif [ "$1" = "-save"  ]; then
        # Save rules to $adblockRules file.
        iptables-save > $adblockRules
        true
        
    elif [ "$1" = "-load"  ]; then
        # Load rules from $adblockRules file.
        iptables-restore < $adblockRules
        true
        
        
    elif [ "$1" = "-reset"  ]; then
        iptables -F OUTPUT
        ip6tables -F OUTPUT
        true
        
        
    elif [ "$1" = "-list"  ]; then
        iptables -L OUTPUT -n
        ip6tables -L OUTPUT -n
        true
        
    elif [ "$1" = "-help"  ]; then
        printf "This script is responsible for creating a simple adblock mechanism. It rejects connections from specific domain names or IP addresses using iptables.\n\n"
        printf "Usage: $0  [OPTION]\n\n"
        printf "Options:\n\n"
        printf "  -domains\t  Configure adblock rules based on the domain names of '$domainNames' file.\n"
        printf "  -ips\t\t  Configure adblock rules based on the IP addresses of '$IPAddresses' file.\n"
        printf "  -save\t\t  Save rules to '$adblockRules' file.\n"
        printf "  -load\t\t  Load rules from '$adblockRules' file.\n"
        printf "  -list\t\t  List current rules.\n"
        printf "  -reset\t  Reset rules to default settings (i.e. accept all).\n"
        printf "  -help\t\t  Display this help and exit.\n"
        exit 0
    else
        printf "Wrong argument. Exiting...\n"
        exit 1
    fi
}

adBlock $1
exit 0