# Building Synthesizers with JUCE

## Chapter 8: The ADSR Envelope

## ADSR

In JX11, the **ADSR Envelope** is a four-stage envelope generator:

| Stage       | JX11 Parameter Name |
| ----------- | ------------------- |
| Attack      | `envAttackParam`    |
| Decay       | `envDecayParam`     |
| Sustain     | `envSustainParam`   |
| Release     | `envReleaseParam`   |

### Attack + Decay

Together, attack and decay form most of the sound's character. For aggressive,
punchy sounds, both the attack and decay should be short. In constrast, pads
and strings use a slow attack and little decay.

#### Attack

The **attack** stage raises the output over a specified time to the full
amplitude. The **`envAttackParam`** value returns a value from 0 to 1 indicating
the amount by which the oscialltor will scale the output. No matter the final
volume of the tone, the attack stage will always end with `envAttackParam`
equal to 1.

#### Decay

The **decay** stage follows immediately after, dropping the sound to
a new level. the **`envDecayParam`** specifies the time it takes for the sound
to decay from its peak to zero. In contast to bounding the lower level of the 
decay to the sustain, another valid approach, the JX11 fully decouples the decay
from the sustain.

### Sustain + Release

Together, sustain and release define the fadeout of the sound.

#### Sustain

In the **sustain** stage, the sound is held at a constant level until a "Note
Off" event is received or the sound enters the release stage. The
**`envSustainParam`** value specifies the level of the sustain, not the time.
Sustain is expressed as a percentage of the note's amplitude.

#### Release

The **release** stage fades the sound down over a specified time. The release
stage is usually long, as many tones naturally fadeout. The
**`envReleaseParam`**, similar to `envDecayParam`, specifies the time it takes
to fade the sound to zero.

The `envReleaseParam` starts from the _maximum amplitude_, not the level of
the sustain. So, changing the level of the sustain changes the release time,
but does not change the release speed.
