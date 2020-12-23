# Security System and Services

## NIKOLAS ZACHARIOUDAKIS 2016030073

7th Assignment: A simple adblocking mechanism.

## Informations

- Every part of the assignment is completed and works properly according to the given details of the assignment.

- I used ip6tables for blocking ipv6 address but due to the fact you didn't want to change the files of the directory, ipv6 address are not writting in adblockRules file. So -laod and -save does not contain ip6tables.

## ANSWER TO THE QUESTION

1. The mechanism we implemented is a firewall, and that means that we can reject every request made by the know ipadresses or domains we have stores in the files. Although, many sites use redirect to their own pages, so we have to block their domain (unable to do that). Another reason is because the browser may use QUIC network protocol like chrome/chromium for the connection. Lastly, another reasons are that we we have not filled the domainNames.txt name or IPAddresses.txt files with every add domain name, or the domain name has changed it's ip address, as there may be mechanism to catch our rejection and it will attempt connection with a different ip address.
