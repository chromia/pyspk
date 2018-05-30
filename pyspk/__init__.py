from ._pyspk import *

# Group.addParticle
def Group_addParticle(group, nb=0, position=None, velocity=None,
                      zone=None, emitter=None, full=True,
                      deltatime=None, step=None, offset=None,
                      start=None, end=None):
    """
    :brief: Adds some Particles to this Group
    
    This method and all the methods of type addParticles will add a given number of Particles at the given position with the given velocity.
    Note that even if a Zone and an Emitter is passed, the position and the velocity will be the same for all Particles.
    
    In case a Zone is passed, Zone::generatePosition(Particle,bool) is used to generate the position.
    In case an Emitter is passed, Emitter::generateVelocity(Particle) with a mass of 1 is used to generate the velocity.
    The velocity will then be updated with the Particle's mass when the Particle will be generated.
    In case a delta time is passed instead of a fixed number, the number will be computed thanks to the flow of the Emitter passed.
    
    Particles will be added to the Group at the next call to update(unsigned int) or flushAddedParticles().
    This is why any Emitter and Zone used to generate particles must be valid at the next call of either update(unsigned int) or flushAddedParticles().
    
    On some methods addParticles, the full variable is present. This variable defines where to generate positions in a Zone :
      true to generate position somewhere within the whole Zone.
      false to generate position somewhere on the Zone border.
    
    :param nb: the number of Particles to add
    :param position: the position where the Particles will be added
    :param velocity: the velocity of the Particles
    """
    if position and velocity:
        if not isinstance(position, Vector3D):
            raise TypeError("position must be Vector3D, not" + str(type(position)))
        elif not isinstance(velocity, Vector3D):
            raise TypeError("velocity must be Vector3D, not" + str(type(velocity)))
        group.addParticles_raw(nb, position, velocity)
    elif zone and emitter:
        if not isinstance(zone, Zone):
            raise TypeError("zone must be a class derived from Zone")
        elif not isinstance(emitter, Emitter):
            raise TypeError("emitter must be a class derived from Emitter")
        group.addParticles_raw(nb, zone, emitter, full)
    elif zone and velocity:
        if not isinstance(zone, Zone):
            raise TypeError("zone must be a class derived from Zone")
        elif not isinstance(velocity, Vector3D):
            raise TypeError("velocity must be Vector3D, not" + str(type(velocity)))
        group.addParticles_raw(nb, zone, velocity, full)
    elif position and emitter:
        if not isinstance(position, Vector3D):
            raise TypeError("position must be Vector3D, not" + str(type(position)))
        elif not isinstance(emitter, Emitter):
            raise TypeError("emitter must be a class derived from Emitter")
        group.addParticles_raw(nb, position, emitter)
    elif zone and emitter and deltatime:
        if not isinstance(zone, Zone):
            raise TypeError("zone must be a class derived from Zone")
        elif not isinstance(emitter, Emitter):
            raise TypeError("emitter must be a class derived from Emitter")
        elif not isinstance(deltatime, float):
            raise TypeError("deltatime is expected as float, not " + str(type(deltatime)))
        group.addParticles_raw(zone, emitter, deltatime, full)
    elif position and emitter and deltatime:
        if not isinstance(position, Vector3D):
            raise TypeError("position must be Vector3D, not" + str(type(position)))
        elif not isinstance(emitter, Emitter):
            raise TypeError("emitter must be a class derived from Emitter")
        elif not isinstance(deltatime, float):
            raise TypeError("deltatime is expected as float, not " + str(type(deltatime)))
        group.addParticles_raw(position, emitter, deltatime)
    elif emitter and deltatime:
        if not isinstance(emitter, Emitter):
            raise TypeError("emitter must be a class derived from Emitter")
        elif not isinstance(deltatime, float):
            raise TypeError("deltatime is expected as float, not " + str(type(deltatime)))
        group.addParticles_raw(emitter, deltatime)
    elif start and end and emitter and step is not None and offset is not None:
        if not isinstance(start, Vector3D):
            raise TypeError("start must be Vector3D, not" + str(type(start)))
        elif not isinstance(end, Vector3D):
            raise TypeError("end must be Vector3D, not" + str(type(end)))
        elif not isinstance(emitter, Emitter):
            raise TypeError("emitter must be a class derived from Emitter")
        elif not isinstance(step, float):
            raise TypeError("step is expected as float, not " + str(type(step)))
        elif not isinstance(offset, float):
            raise TypeError("offset is expected as float, not " + str(type(offset)))
        return group.addParticles_raw(start, end, emitter, step, offset)
    elif start and end and velocity and step is not None and offset is not None:
        if not isinstance(start, Vector3D):
            raise TypeError("start must be Vector3D, not" + str(type(start)))
        elif not isinstance(end, Vector3D):
            raise TypeError("end must be Vector3D, not" + str(type(end)))
        elif not isinstance(velocity, Vector3D):
            raise TypeError("velocity must be Vector3D, not" + str(type(velocity)))
        elif not isinstance(step, float):
            raise TypeError("step is expected as float, not " + str(type(step)))
        elif not isinstance(offset, float):
            raise TypeError("offset is expected as float, not " + str(type(offset)))
        group.addParticles_raw(start, end, velocity, step, offset)
    elif emitter:
        if not isinstance(emitter, Emitter):
            raise TypeError("emitter must be a class derived from Emitter")
        group.addParticles_raw(nb, emitter)
    else:
        raise ArgumentError("not supported call type")

Group.addParticles = Group_addParticle
