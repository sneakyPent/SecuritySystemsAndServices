#!/bin/bash
# You are NOT allowed to change the files' names!
domainNames="domainNames.txt"
IPAddresses="IPAddresses.txt"
adblockRules="adblockRules"

function adBlock() {
    if [ "$EUID" -ne 0 ];then
        printf "Please run as root.\n"
        exit 1
    fi
    if [ "$1" = "-domains"  ]; then
        n=1    
        while IFS= read -r line
        do
            if iptables -A OUTPUT -d $line -j REJECT  > /dev/null 2>&1; then
               : $((n++))
            else
                if ! ip6tables -A OUTPUT -d $line -j REJECT  > /dev/null 2>&1; then
                    echo "Domain name \"$line\" not found and forced from the list deleted."
                    sed -i "$n d" $input
                fi
            fi
            # nslookup $line | grep ^Name -A1| awk '{print $2}' | awk 'NR%2==0' >> $IPAddresses
            host $line | egrep -o "[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}|([a-f0-9:]+:+)+[a-f0-9]+" >> $IPAddresses
        done < "$domainNames"

        while IFS= read -r line
        do
            nslookup $line | grep ^Name -A1| awk '{print $2}' | awk 'NR%2==0' >> $IPAddresses
        done < "$domainNames"
        true
            
    elif [ "$1" = "-ips"  ]; then
        while IFS= read -r line
        do
            if ! iptables -A OUTPUT -d $line -j REJECT  > /dev/null 2>&1; then
                ip6tables -A OUTPUT -d $line -j REJECT
            fi
        done < "$IPAddresses"
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
        # Reset rules to default settings (i.e. accept all).
        iptables -F OUTPUT
        ip6tables -F OUTPUT
        true

        
    elif [ "$1" = "-list"  ]; then
        # List current rules.
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