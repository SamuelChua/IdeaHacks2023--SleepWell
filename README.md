# IdeaHacks2023--SleepWell
Say goodbye to restless nights and hello to rejuvenating, restful sleep with SleepWell! It features temperature, humidity, light and sound to determine your sleep quality and outsleep your friends!

# SleepWell Device 
<img width="200" alt="IdeaHacks_1" src="https://user-images.githubusercontent.com/9492646/214800708-29f5f14e-950a-499d-8430-5acaa8c1729b.png">
<img width="200" alt="IdeaHacks" src="https://user-images.githubusercontent.com/9492646/214800711-203c6707-9a2d-4b19-b84d-93ab066dcd92.png">

# Overview of Circuit Setup in SleepWell
<img width="300" alt="Sleepwell_mechanism" src="https://user-images.githubusercontent.com/9492646/214800712-8f418c7a-73fa-44d7-8de7-27ea374b7d41.png">

# Dashboard of SleepWell Web Server 
<img width="300" alt="IdeaHacks_DashBoard" src="https://user-images.githubusercontent.com/9492646/214800728-1553e0a9-bbc2-4871-b51e-483a37925662.png">

# Inspiration
Sleep is often lacking in our fast-paced environment. With an increasing emphasis on mental health and wellness since the COVID-19 pandemic, sleep plays an even crucial role in maintaining healthy lifestyles and relationships. Therefore, we wanted to gamify this daily habit of sleeping so that people can work towards having better quality sleep.

# What it does
SleepWell tracks features such as temperature, humidity, light and sound levels so as to determine how conducive the environment is for sleeping. The data is shown on a website users can see to compare their sleeping scores and see who is the king of sleep. We implemented a user-friendly manner to feel the numbers and tell the time in case they experience sleeping problems in the middle of the night.

# How we built it
We used various sensors to track temperature, humidity, light and sound levels and linked it to the ESP32 Dev Kit to derive readings. These readings are transferred via HTTP to a website we hosted ourselves and built using HTML5 and CSS3. The structure of SleepWell is laser-cut with the numbers on the clock with a rotatable plate for the users to feel the time in the night.

# Challenges we ran into
We ran into couple of challenges, firstly getting the servo motor to spin 360 degrees at a speed that models that of the hour hand of a clock. Secondly, the integration of multiple sensors and making sense of some of the arbitrary values was a challenge since we will be displaying that information to the users. Lastly, we wanted to have a mobile application for users to see on the Android Play Store, but because of Android Studio and driver issues, we switched to hosting it on a website.

# Accomplishments that we're proud of
Having a bruin-themed cut out for SleepWell and having a final product to present from the hackathon. Also, we're proud of getting all the sensors to work properly in one breadboard and displaying that information in an aesthetic manner to the users.

# What we learned
We learned about having to be patient and isolating the problem especially when dealing with circuits. Such debugging techniques will be useful when faced with circuitry issues. Moreover, having a baseline product that we can fall back on will be useful in a hackathon when things do not go according to plan.

# What's next for SleepWell
SleepWell will be in a Play Store near you, and based on user's sleep ratings on the dashboard, SleepWell will personalize your sleep and make recommendations on how to further improve your sleep using machine learning models. SleepWell will be integrated with servos and speakers to tell the time. Additionally, to incentivize users to get the appropriate amount of sleep, SleepWell will be getting users to play a mini-game by placing their fingers on a touch sensor according to the beat of the flashing LED light on the box. This will add bonus points for being alert and getting out of their sleeping phase.
