# GiveMiiSonic
This is a patch that enables automatic booting of a debug menu for the 2016 video game "Mario & Sonic at the Rio 2016 Olympic Games" on the Wii U... But only for the European and USA copies of the game.

## Special Thanks to
[@GaryOderNichts](https://github.com/GaryOderNichts) for some guidance, and for making [GiveMiiYoutube](https://github.com/PretendoNetwork/GiveMiiYouTube) in the first place that I then modified.
[@ashquarky](https://github.com/ashquarky) for the awesome patcher framework which was taken from [Nimble](https://github.com/PretendoNetwork/Nimble).
[@vgmoose](https://github.com/vgmoose) for initially helping me get this working from GiveMiiYoutube's base.

## Building
```bash
# Build docker image (only needed once)
docker build . -t givemiisonic_builder

# make 
docker run -it --rm -v ${PWD}:/project givemiisonic_builder make

# make clean
docker run -it --rm -v ${PWD}:/project givemiisonic_builder make clean
```
