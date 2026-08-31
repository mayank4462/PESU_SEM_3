## Q1. Define a computer network.

### Answer:

A computer network is a collection of **two or more connected devices** that can communicate with each other and share data or resources.

---

## Q2. Define the Internet.

### Answer:

The Internet is an **immense network of networks** that interconnects billions of computing devices throughout the world.

---

## Q3. What is a host?

### Answer:

A **host** is a connected computing device, also called an **end system**, located at the edge of the Internet.

Examples include PCs, smartphones and servers.

---

## Q4. What is a packet?

### Answer:

A **packet is a chunk of data** that is transmitted through a computer network.

---

## Q5. What is a packet switch?

### Answer:

A packet switch is a network device that **forwards packets** toward their destination.

Examples include:

- Router
- Switch

---

## Q6. What is bandwidth?

### Answer:

Bandwidth is the **transmission rate of a communication link**.

---

## Q7. Name four types of communication links.

### Answer:

Four communication links mentioned are:

1. Fiber
2. Copper
3. Radio
4. Satellite

---

## Q8. What is a protocol?

### Answer:

A protocol is a set of rules governing communication between network entities.

It specifies:

- Message format
- Message order
- Actions taken during transmission and reception

---

## Q9. What are the two important organizations mentioned in relation to Internet standards?

### Answer:

- **RFC — Request for Comments**
- **IETF — Internet Engineering Task Force**

---

## Q10. What does RFC stand for?

### Answer:

**RFC = Request for Comments**

---

## Q11. What does IETF stand for?

### Answer:

**IETF = Internet Engineering Task Force**

---

## Q12. Give four examples of Internet protocols mentioned in the slides.

### Answer:

Any four:

- TCP
- IP
- HTTP
- Wi-Fi
- 4G/5G
- Ethernet

---

## Q13. Give four examples of Internet-connected devices.

### Answer:

Any four:

- Smartphone
- Laptop
- Smart TV
- Gaming console
- Home appliance
- Smartwatch
- Car
- Security camera

---

## Q14. What is meant by "network of networks"?

### Answer:

It means that the Internet is formed by **many interconnected networks**, including home networks, mobile networks, enterprise networks and ISP networks.

---

## Q15. What is the programming interface provided by Internet infrastructure?

### Answer:

It is an interface that allows distributed applications to **hook into the Internet**, send and receive messages, and use Internet transport services.

---

# PART 4 — 4-MARK QUESTIONS

These need slightly longer answers.

---

# Q1. Explain the Internet as a "network of networks."

### Answer:

The Internet is an **immense network of networks** that connects billions of computing devices throughout the world.

It consists of many interconnected networks, such as:

- Home networks
- Mobile networks
- Enterprise networks
- Local/regional ISPs
- National/global ISPs
- Content provider networks

These networks are connected using **routers, packet switches and communication links**.

```
Home Network ──┐
               │
Enterprise ────┼── ISP ── ISP ── Global Internet
               │
Mobile Network ┘
```

Therefore, the Internet is not a single network but a collection of interconnected networks.

---

# Q2. Explain the basic components of the Internet from the nuts-and-bolts view.

### Answer:

The basic components include:

### 1. Hosts / End Systems

Connected computing devices such as PCs, smartphones and servers.

### 2. Packet Switches

Devices such as routers and switches that forward packets.

### 3. Packets

Chunks of data transmitted through the network.

### 4. Communication Links

Connections between devices and networks. Examples include fiber, copper, radio and satellite.

### 5. Networks

Collections of devices, routers and links managed by organizations.

### 6. ISPs

Interconnected Internet Service Provider networks form an important part of the Internet infrastructure.

---

# Q3. Explain network protocols.

### Answer:

A network protocol is a set of rules that governs communication between network entities.

A protocol specifies:

1. **Format** of messages
2. **Order** in which messages are sent and received
3. **Actions** taken when a message is transmitted
4. **Actions** taken when a message is received

Examples of protocols mentioned in the slides include:

- TCP
- IP
- HTTP
- Wi-Fi
- 4G/5G
- Ethernet

### Key line:

> **Protocols define the format, order and actions associated with communication.**

---

# Q4. Explain the difference between human protocols and network protocols.

### Answer:

A human protocol governs communication between humans, whereas a network protocol governs communication between computers/devices.

### Human protocol

Example:

```
A: Hi
B: Hi
A: Got the time?
B: 2:00
```

There is an expected sequence of messages and responses.

### Network protocol

A computer network follows formal rules.

For example:

```
Client → TCP connection request → Server
Client ← TCP connection response ← Server
Client → GET → Server
Client ← Requested resource ← Server
```

Thus, both human and network communication require rules regarding **messages, order and responses**, but network protocols are formally defined for computer communication.

---

# Q5. Explain communication links and transmission rate.

### Answer:

A **communication link** is a medium through which data is transferred between network devices.

The slides mention:

- Fiber
- Copper
- Radio
- Satellite

Every communication link has a **transmission rate**, also called **bandwidth**.

Therefore:

```
Communication Link
        ↓
Fiber / Copper / Radio / Satellite
        ↓
Transmission Rate
        ↓
Bandwidth
```

Bandwidth represents how quickly data can be transmitted over the communication link.

---

# Q6. Explain packet switching.

### Answer:

In packet switching, data is divided into **packets**, where each packet is a chunk of data.

The packets are forwarded through the network using **packet switches**, such as:

- Routers
- Switches

A simplified flow is:

```
Source
  ↓
Packet
  ↓
Router
  ↓
Router
  ↓
Destination
```

The Internet uses packets to move data between connected devices.

> **Note:** The pages provided introduce packet switching and packet switches, but do not provide the detailed packet-switching mechanisms or calculations. Do not add those details to an answer if your question is specifically restricted to these slides.

---

# Q7. Explain the role of Internet infrastructure.

### Answer:

Internet infrastructure provides services that support many distributed applications.

Applications mentioned include:

- Web
- Video
- Streaming video
- Multimedia
- Teleconferencing
- Email
- Games
- Social media
- Inter-connected appliances

The infrastructure also provides a **programming interface** through which distributed applications can connect to the Internet and send/receive messages.

---

# Q8. Explain Internet-connected devices with examples.

### Answer:

The Internet connects traditional computing devices as well as many other devices.

### Traditional devices

- PCs
- Workstations
- Servers

### Other connected devices

- Laptops
- TVs
- Gaming consoles
- Home security systems
- Home appliances
- Watches
- Cars
- Traffic-control systems

The slides additionally show devices such as:

- Amazon Echo
- Fitbit
- Security cameras
- Internet refrigerators
- Internet phones
- AR devices
- Sensorized mattresses

This shows that Internet connectivity extends beyond conventional computers.

---

# PART 5 — VERY IMPORTANT DIFFERENCES

## Host vs Packet Switch

|Host / End System|Packet Switch|
|---|---|
|Located at the network edge|Used within the network|
|Source/destination of communication|Forwards packets|
|Examples: PC, phone, server|Examples: router, switch|

### Remember:

**Host → sends/receives data**

**Router/Switch → forwards packets**

---

# Protocol vs Communication Link

|Protocol|Communication Link|
|---|---|
|Rules for communication|Medium/path for communication|
|Defines message format/order/actions|Carries data|
|Examples: TCP, IP, HTTP|Examples: fiber, copper, radio, satellite|

---

# Packet vs Packet Switch

|Packet|Packet Switch|
|---|---|
|Chunk of data|Network device|
|Travels through network|Forwards packets|
|Example: data packet|Router/switch|

---

# Internet vs Computer Network

|Computer Network|Internet|
|---|---|
|Two or more connected devices|Network of networks|
|Can be small or large|Global|
|Can be a home/enterprise network|Connects many networks|
|Shares data/resources|Interconnects billions of devices|

---

# PART 6 — FILL IN THE BLANKS

These are good for quick revision.

### 1.

The Internet is a **__________ of networks**.

**Answer:** network

### 2.

Hosts are also called **__________ systems**.

**Answer:** end

### 3.

A packet is a **__________ of data**.

**Answer:** chunk

### 4.

Routers and switches are examples of **__________ switches**.

**Answer:** packet

### 5.

Transmission rate is also called **__________**.

**Answer:** bandwidth

### 6.

Fiber, copper, radio and satellite are types of **__________ links**.

**Answer:** communication

### 7.

RFC stands for **__________**.

**Answer:** Request for Comments

### 8.

IETF stands for **__________**.

**Answer:** Internet Engineering Task Force

### 9.

HTTP, TCP and IP are examples of Internet **__________**.

**Answer:** protocols

### 10.

Protocols define the format and **__________** of messages.

**Answer:** order

### 11.

A router is a type of **__________ switch**.

**Answer:** packet

### 12.

The Internet infrastructure provides a **__________ interface** for distributed applications.

**Answer:** programming

---

# PART 7 — TRUE OR FALSE

### 1. The Internet is a single network.

**False**

It is a **network of networks**.

### 2. Hosts are also called end systems.

**True**

### 3. Routers forward packets.

**True**

### 4. A packet is a physical networking device.

**False**

A packet is a **chunk of data**.

### 5. Bandwidth refers to transmission rate.

**True**

### 6. Fiber can be a communication link.

**True**

### 7. Protocols only define message format.

**False**

They define **format, order and actions**.

### 8. TCP is a protocol.

**True**

### 9. Fitbit is an example of an Internet-connected device shown in the slides.

**True**

### 10. IETF stands for Internet Engineering Task Force.

**True**

---

# PART 8 — HIGH-PROBABILITY 4-MARK QUESTION

## "Explain the Internet using a suitable diagram."

Draw this in the exam:

```
                    INTERNET
                       |
              Network of Networks
                       |
       ┌───────────────┼───────────────┐
       ↓               ↓               ↓
 Home Network    Mobile Network   Enterprise Network
       │               │               │
       └───────────────┼───────────────┘
                       ↓
                      ISP
                       |
                National/Global ISP
                       |
                    Routers
                       |
                 Communication
                     Links
              /        |        \
           Fiber     Copper    Radio
                       |
                     Hosts
               /       |       \
              PC     Phone     Server
```

Then explain:

> The Internet is an immense network of networks. It consists of billions of hosts/end systems connected through communication links and packet switches. Different networks such as home, mobile and enterprise networks are interconnected through ISPs. Data is transmitted as packets, and protocols govern communication between network entities.

That is a strong **4-mark answer**.

---

# PART 9 — LAST-MINUTE REVISION SHEET

If you're walking into the exam and have **5 minutes**, memorize this:

```
COMPUTER NETWORK
= 2 or more connected devices
= communication + data/resource sharing

INTERNET
= network of networks
= billions of connected devices

HOST
= END SYSTEM
= device at network edge

PACKET
= chunk of data

PACKET SWITCH
= forwards packets
= Router / Switch

COMMUNICATION LINKS
= Fiber
= Copper
= Radio
= Satellite

TRANSMISSION RATE
= BANDWIDTH

PROTOCOL
= rules governing communication
= Format
= Order
= Actions

PROTOCOL EXAMPLES
= TCP
= IP
= HTTP
= Wi-Fi
= 4G/5G
= Ethernet

INTERNET STANDARDS
= RFC
= IETF

INTERNET INFRASTRUCTURE
supports:
Web
Video
Streaming
Multimedia
Teleconferencing
Email
Games
Social Media

PROGRAMMING INTERFACE
= allows distributed applications
  to hook into the Internet
  and send/receive messages
```

## 🔥 Top 10 things I would memorize

1. **Internet = network of networks**
2. **Host = end system**
3. **Packet = chunk of data**
4. **Router/switch = packet switch**
5. **Bandwidth = transmission rate**
6. **Fiber/copper/radio/satellite = communication links**
7. **Protocol = format + order + actions**
8. **TCP/IP/HTTP/Wi-Fi/Ethernet = protocols**
9. **RFC = Request for Comments**
10. **IETF = Internet Engineering Task Force**

These cover a very large portion of what can reasonably be asked from **pages 10–20**. The later slides in the PDF only introduce topics such as **network edge, network core, circuit switching, delay/loss/throughput, protocol layers, OSI and TCP/IP**, but they do **not actually teach those topics yet** in the provided pages, so I would not invent detailed exam answers for them from this section.