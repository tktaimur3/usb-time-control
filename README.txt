This is an embedded project I worked on to solve a problem I had over the summer. It's made with an Arduino Nano.

Over the summer, I always kept my fan on all night to help me fall asleep (the white noise). But when I woke up I would notice that it was always 
freezing cold. I knew it was the fan and I thought of a way to solve it, but as I was working over the summer, I didn't have much time to execute my 
plan.

Well now I pretty much finished it, and it's not exactly how I expected it to turn out but regardless I'm pretty happy with it. Instead of just powering
a fan, it can power any USB device, hence the name. It allows you to pick a specific amount of hours and minutes ranging from 0-59 for minutes and 
0-23 for hours. The range was selected somewhat arbritrarily, reason being that I doubt I want to leave my fan on for more than a day. Also, the 10-bit
analog voltage input on the Arduino didn't allow for much precision.

The time can be seen through a small OLED screen which looks pretty aesthetically pleasing. After pressing a pushbutton, you can see the time count down 
and simultaneously, the power is delivered to the USB port. Powering the USB was somewhat of a challenge, due to the fact that the Arduino can only 
output a maximum of 40mA from any one of its pins (500mA is standard for USB). Due to this, I had to use a Darlington transisor to amplify the current. 
It took some trial and error since the transistors I had originally weren't rated for higher currents and so they would start burning up if I wired my 
own Darlington. Funny enough, their maximum collector current was 100mA, meanwhile I was trying to push 500mA wondering why they would start getting hot.

The code is some string manipulation logic along with counting. To drive the OLED screen, I used U8G2 libaries: https://github.com/olikraus/u8g2 
which were a big help in terms of improving the aesthetics of my project (it has so many fonts!). I was originally using the bulky liquid crystal 
display which looked really awkward and had, like, 20 wires. In comparison, the OLED screen has 4!

This project was made in about a week and helped me learn a lot more about embedded system. Designing and implementing one takes many comprimises and 
tough decisions, so it definitely gave me more insight into the engineering design process.