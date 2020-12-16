# Security System and Services

## NIKOLAS ZACHARIOUDAKIS 2016030073

6th Assignment: Monitoring the network traffic using the Packet Capture library.

## Informations

* Every part of the assignment is completed and works properly according to the given details of the assignment, except for the 11th step "In your program (when possible), mark each retransmitted packet as “Retransmitted”. ".
* If you want any specification for the implemented functions, you can find them in the header files given as function declerations.

## Usage

```text
usage:      ./monitor -option [option argument]

options:
        -i,     Name of the network interface will be monitored.
        -r,     The Packet capture will be used (file name)
        -h,     Help message

```

## ANSWERS FOR 9th AND 10th QUESTIONS

9. In TCP we are able to tell if a packet is retransmitted. The mechanism works as follows. First there is a triple handshake connection as we show in class too. Next, both server and client have sequence and acknowledgement params in ther tcp headers for every packet they send to each other. So to check if a packet is retransmitted we should check its conversation independently. For a scenario where we have one server-client conversation we would operate as follows. After the handshake, when server receives a package he increase his acknowledgement param by the received payload len [ ack = ack + paylaod_len ]. In this way when server sends a package back to the client, the second will check the received acknowledgement param whether it's the same as his current sequence number or not. If not, the package will probably be a retransmitted package. Moreover, we know that the sequence number client/server will check contrary to the received acknowledgment is the last seq increased by the last packet's payload length.

10. We know that UD Protocol does not use retransmissions, because it does not care whether the packet will be received from destinaiton address or not. So there are no retransmitted packets in UDP.
