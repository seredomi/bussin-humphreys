# humphreys_buses

The goal of this project is to create a program that can calculate optimal bus routes on Camp Humphreys

I'm taking on this project because it seems like an obvious and useful piece of software that I feel has been missing since I got here.
I had the idea to start on it in mid 2022, but I was too busy with school and work to ever get around to it.
Now that I have some free time between Spring and Summer classes, I've finally been able to make some progress...

Here's the plan:
- [x] collect coordinates, descriptions, pictures, and any other metadata for all 55 bus stops on base (I ride my bike around base from stop to stop and record all this)
- [x] collect route info including schedule, stupos, and timing (I ride the buses repeatedly and use an app called GPX Tracker Pro to record their movements)
- [x] distill all this info into CSV files that can be edited later on to reflect changes
- [x] program reads these files and creates useful and efficient data structures out of them
- [ ] program returns possible routes where bus stops are within 2km of start/end coords
- [ ] program that sorts these possible routes
- [ ] simple webapp to interact with these algorithms
- [ ] safely allow users to use device location
- [ ] integrate an interactive map that shows bus stops
- [ ] add routes to map
- [ ] add other building numbers / locations to map
