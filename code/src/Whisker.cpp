#include "Whisker.hpp"

Whisker::Whisker(btDiscreteDynamicsWorld* world, GUIHelperInterface* helper,btAlignedObjectArray<btCollisionShape*>* shapes, std::string w_name, int w_index, Parameters* parameters){
		color = btVector4(0.1, 0.1, 0.1, 1);
		// save parameters and global variables to whisker object
		m_collisionShapes = shapes;	// shape vector pointer
		m_dynamicsWorld = world;	// simulation world pointer
		m_guiHelper = helper;

		m_index = w_index;
		friction = 0.5;
		m_angle = 0.;		// initialize protraction angle
		m_time = 0;			// initialize time
		ACTIVE = parameters->ACTIVE;
		NO_MASS = parameters->NO_MASS;
		BLOW = parameters->BLOW;
		PRINT = parameters->PRINT;
		dt = parameters->TIME_STEP;
		NUM_LINKS = parameters->NUM_UNITS;
		NUM_JOINTS = NUM_LINKS - 1;

		// initialize collide array
		std::vector<int> all_zeros(NUM_LINKS, 0);
		collide = all_zeros;
		dphi = {0.398f,0.591f,0.578f,0.393f,0.217f};
		dzeta = {-0.9f,-0.284f,0.243f,0.449f, 0.744f};

		//Whisker specific configuration parameters
		whisker_config config = get_config(w_name, parameters);
		side = config.side;
		row = config.row;
		col = config.col;
		length = config.L*SCALE;
		link_length = length/btScalar(NUM_LINKS);
		radius_base = calc_base_radius(row, col, length); // base radius
		radius_slope = calc_slope(length, radius_base, row, col);
		radius_tip = radius_base - length*radius_slope;
		link_angles = config.link_angles;
		base_pos = config.base_pos;
		base_rot = config.base_rot;

		//Whisker universal configuration parameters
		rho = parameters->RHO_BASE/pow(SCALE,3);	// rho: density
		rho_slope = ((parameters->RHO_TIP-parameters->RHO_BASE)/pow(SCALE,3)) / length;
		zeta = parameters->ZETA_BASE;				// zeta: damping ratio
		E = parameters->E_BASE*1e9/SCALE;			// E: Young's modulus
}

whisker_config Whisker::get_config(std::string wname,Parameters* parameters){
    
    boost::filesystem::path full_path(boost::filesystem::current_path());
    // read in parameter file
    std::vector<std::string> whisker_names;
    std::vector<std::vector<int>> whisker_pos;
    std::vector<std::vector<float>> whisker_geom;
    std::vector<std::vector<float>> whisker_angles;
    std::vector<std::vector<float>> whisker_bp_coor;
    std::vector<std::vector<float>> whisker_bp_angles;

    std::string file_angles = "../data/param_angles.csv";
    read_csv_string("../data/param_name.csv",whisker_names);
    read_csv_int("../data/param_side_row_col.csv",whisker_pos);
    read_csv_float("../data/param_s_a.csv",whisker_geom);
    read_csv_float(file_angles,whisker_angles);
    read_csv_float("../data/param_bp_pos.csv",whisker_bp_coor);
    read_csv_float("../data/param_bp_angles.csv",whisker_bp_angles);
    whisker_config wc;
    for(int i=0;i<whisker_names.size();i++){
        if(!wname.compare(whisker_names[i])){
            
            wc.id = wname;
            wc.side = whisker_pos[i][0];
            wc.row = whisker_pos[i][1];
            wc.col = whisker_pos[i][2];
            wc.L = whisker_geom[i][0]/1000.;
            wc.a = whisker_geom[i][1]*1000.;
            wc.link_angles = whisker_angles[i];
            wc.base_pos = btVector3(whisker_bp_coor[i][0],whisker_bp_coor[i][1],whisker_bp_coor[i][2])/1000.*SCALE;
            wc.base_rot = btVector3(whisker_bp_angles[i][0]-PI/2,-whisker_bp_angles[i][1],whisker_bp_angles[i][2]+PI/2);
            break;
        }
    }
    return wc;
    
}

// function to create whisker, from head, then offset to origin
void Whisker::buildWhisker(btRigidBody* head, btTransform head2origin){
	/* Remark:
	The reason why some of these rigid bodies has mass, where they're really supposed
	to be a kinematic object, is that setting mass to zero will make them static. 
	This could potentially be a bug in Bullet engine, or misuse of code that can be
	implemented correctly. It is worth looking back at this problem only if the current
	solution (use large mass) will result in non-negligible result.
	*/

	/// CREATE BASE POINT
	/// This is a box shape that is only translated from origin to basepoint location.
	/// It's body frame is axis-aligned.
	/// ====================================
	// originTransform is the mean location of the whisker array
	btTransform originTransform = head->getCenterOfMassTransform()*head2origin;
	// basepointTransform is the location of the basepoints
	btTransform basepointTransform = originTransform*createFrame(base_pos);

	// Notice: the collision shape for the basepoint is btBoxShape (arbitrary choice)
	//    	   4*radius is for visual/debugging purpose
	// New question: basepoint and whisker base are overlapping, won't this collision
	// affect the simualtion process?
	btCollisionShape* basepointShape = new btBoxShape(4*btVector3(radius_base, radius_base, radius_base));
	m_collisionShapes->push_back(basepointShape);
	basepoint = createDynamicBody(btScalar(100), friction, basepointTransform, basepointShape, m_guiHelper, color);
	// add basepoint rigid body to the world
	m_dynamicsWorld->addRigidBody(basepoint,COL_BASE,baseCollidesWith);
	basepoint->setActivationState(DISABLE_DEACTIVATION);

	// why create new transform for both, and set linear limit to 0?
	btVector3 head2basepoint = head2origin.getOrigin() + base_pos;
	btTransform inFrameA = createFrame();
    btTransform inFrameB = createFrame();
	basePointConstraint = new btGeneric6DofConstraint(*head, *basepoint, inFrameA, inFrameB, true);
	basePointConstraint->setLinearLowerLimit(head2basepoint);
	basePointConstraint->setLinearUpperLimit(head2basepoint);
	basePointConstraint->setAngularLowerLimit(btVector3(0,0,0));
	basePointConstraint->setAngularUpperLimit(btVector3(0,0,0));

	m_dynamicsWorld->addConstraint(basePointConstraint,true);
	basePointConstraint->setDbgDrawSize(btScalar(0.5f));

    /// WHISKER BASE
	/// This is a sphere shape that has exactly the same transform as the basepoint (box).
	/// In non-whisking mode, it's body frame is axis-aligned. In whisking mode, this node
	/// serves as a moving node that receives angular velocity parameter from Knutsen.
	/// =========================================================== 
	// Now, basepointTransform become the absolute transform of the basepoint
	btTransform baseTransform = basepoint->getCenterOfMassTransform();
	// Notice: the collision shape for the whisker base is btSphereShape (arbitrary choice)
	//    	   5*radius is for visual/debugging purpose
	// btCollisionShape* baseShape = createSphereShape(radius_base*5);
	btCollisionShape* baseShape = new btSphereShape(radius_base*5);
	m_collisionShapes->push_back(baseShape);
	base = createDynamicBody(btScalar(10),friction,baseTransform,baseShape,m_guiHelper,color);
	// add whisker base rigid body to the world
	m_dynamicsWorld->addRigidBody(base,COL_BASE,baseCollidesWith);
	base->setActivationState(DISABLE_DEACTIVATION);

	// add constraint between basepoint and whisker base. (motor constraint)
	motorConstraint = new btGeneric6DofConstraint(*basepoint, *base, inFrameA, inFrameB, true);

	// set angular limit of this motor constraint, and add it to the world
	// this constraint is relative to the basepoint
	btVector3 lowerLimit;
	btVector3 upperLimit;
	// if in ACTIVE mode, use dynamic range
	if (ACTIVE) {	
		if(!side){ 
			lowerLimit = btVector3(-PI/6,-PI/6,-PI/6);
			upperLimit = btVector3(PI/3,PI/3,PI/3);
		}
		else{
			lowerLimit = btVector3(-PI/3,-PI/3,-PI/3);
			upperLimit = btVector3(PI/6,PI/6,PI/6);
		}
	// if not in ACTIVE mode, use static range
	} else {
		if(!side){ // dynamic range
			lowerLimit = btVector3(0, 0, 0);
			upperLimit = btVector3(0, 0, 0);
		}
		else{
			lowerLimit = btVector3(0, 0, 0);
			upperLimit = btVector3(0, 0, 0);
		}
	}
	motorConstraint->setLinearLowerLimit(btVector3(0,0,0));
	motorConstraint->setLinearUpperLimit(btVector3(0,0,0));
	motorConstraint->setAngularLowerLimit(lowerLimit);
	motorConstraint->setAngularUpperLimit(upperLimit);

	m_dynamicsWorld->addConstraint(motorConstraint,true);
	motorConstraint->setDbgDrawSize(btScalar(0.5f));
	/// BUILD WHISKER
	/// This consists of several frustum links that are connected by torsinal springs.
	/// The first link is manually rotated to align with the whisker emergence angles.
	/// From then on, every following link is rotated by a link_angle, to simualte the
	/// correct whisker curvature.
	/// ===========================================================
	// Known: radius_base, radius_slope, radius_tip
	btScalar radius;
    btScalar radius_next = radius_base;
    btRigidBody* link_prev = base; 
	
	//
    for(int i=0;i< NUM_LINKS;++i) {
		// prepare parameters for this link
        radius = radius_next;
        radius_next = radius - link_length * radius_slope;
        btScalar angle = link_angles[i];

        // calculate parameters of the whisker
        rho = rho + rho_slope*link_length;
        btScalar mass = calc_mass(link_length, radius, radius_next, rho); // in kg
        if (NO_MASS){
            mass = 0.;
        }
		// com: the center of mass of the remaining part of the whisker
        btScalar com = calc_com(link_length, radius, radius_next); // in cm
        btScalar vol = calc_volume(link_length, radius, radius_next); // in cm
        btScalar inertia = calc_inertia(radius); // in cm4
        btScalar stiffness = calc_stiffness(E,inertia,length);

        btScalar com_distal = calc_com((NUM_LINKS-i)*link_length, radius, radius_tip); // in cm
        btScalar mass_distal = calc_mass((NUM_LINKS-i), radius, radius_tip, rho); // in kg
        btScalar damping = calc_damping(stiffness, mass_distal, com_distal, zeta, dt);

        // Notice: the collision shape for each link/unit is btTruncatedConeShape
		// Be careful about the memory overflow, check deconstructor
        btTruncatedConeShape* linkShape = new btTruncatedConeShape(radius*BLOW, radius_next*BLOW, link_length,0);
        linkShape->setMargin(0.0001);
        m_collisionShapes->push_back(linkShape);

        // set position and rotation of current unit
        btTransform prevTransform = link_prev->getCenterOfMassTransform();
        
        btTransform totalTransform;
		// if current link is the 1st link, then
		// rotation : base_rot
		// translation: link_length/2
        if(i==0){
            btTransform rotTransform = rotZ(base_rot[0])*rotY(base_rot[1])*rotX(base_rot[2]);
            btTransform transTransform = createFrame(btVector3(link_length/2.f,0,0));
            totalTransform = prevTransform*rotTransform*transTransform;
        }
		// if current link is successive link
		// translation: link_length/2
		// rotation: link_angle[i]
		// translation: link_length/2
        else{
            btTransform linkTransform1 = createFrame(btVector3(link_length/2.f,0,0));
            btTransform linkTransform2 = createFrame(btVector3(0,0,0),btVector3(0,0,angle));
            btTransform linkTransform3 = createFrame(btVector3(link_length/2.f,0,0));
            totalTransform = prevTransform*linkTransform1*linkTransform2*linkTransform3;
        }

        // add link to whisker and world
        btRigidBody* link = createDynamicBody(mass, friction, totalTransform, linkShape, m_guiHelper, color);
        whisker.push_back(link);	
        
        if(side){ 
            m_dynamicsWorld->addRigidBody(link,COL_ARRAY_R,arrayRCollidesWith); 
        }
        else{
            m_dynamicsWorld->addRigidBody(link,COL_ARRAY_L,arrayLCollidesWith);
        }
		// set the pointer of this link to one element of vector<int>
        link->setUserPointer(&collide[i]);
		// activate this link
        link->setActivationState(DISABLE_DEACTIVATION);
		
		// add constrints between the 
		// the rotated center of whisker base and
		// the center of current link's start
        if(i==0){
             // initialize transforms and set frames at end of frustum
            btTransform frameInCurr = createFrame(btVector3(-(link_length/2.f),0,0));
            btTransform frameInPrev = rotZ(base_rot[0])*rotY(base_rot[1])*rotX(base_rot[2]);
            baseConstraint = new btGeneric6DofConstraint(*link_prev, *link, frameInPrev, frameInCurr, true);
			
			// the whisker emerges from the face at a given angle
			// the whisker were not able to move relative to the base
            baseConstraint->setLinearLowerLimit(btVector3(0,0,0));
            baseConstraint->setLinearUpperLimit(btVector3(0,0,0));
            baseConstraint->setAngularLowerLimit(btVector3(0,0,0));
            baseConstraint->setAngularUpperLimit(btVector3(0,0,0));
            
            m_dynamicsWorld->addConstraint(baseConstraint,true);
            baseConstraint->setDbgDrawSize(btScalar(0.5f));
        
            // enable feedback (mechanical response)
            baseConstraint->setJointFeedback(&baseFeedback);
        }
		// add constraints between 
		// the center of previous link's end and
		// the center of current link's start
        else{
            btTransform frameInPrev = createFrame(btVector3(link_length/2.f,0,0),btVector3(0,0,0));
		    btTransform frameInCurr = createFrame(btVector3(-link_length/2.f,0,0),btVector3(0,0,0));
        
            // create link (between units) constraint
            btGeneric6DofSpringConstraint* spring = new btGeneric6DofSpringConstraint(*link_prev, *link, frameInPrev,frameInCurr,true);

            // set spring parameters of links
            // ----------------------------------------------------------		
            spring->setLinearLowerLimit(btVector3(0,0,0)); // lock the units
            spring->setLinearUpperLimit(btVector3(0,0,0));
            spring->setAngularLowerLimit(btVector3(0.,1.,1.)); // lock angles between units at x axis but free around y and z axis
            spring->setAngularUpperLimit(btVector3(0.,0.,0.));

            // add constraint to world
            m_dynamicsWorld->addConstraint(spring, true); // true -> collision between linked bodies disabled
            spring->setDbgDrawSize(btScalar(0.5f));

            spring->enableSpring(4,true);
            spring->setStiffness(4,stiffness);
            spring->setDamping(4,damping);
            spring->setEquilibriumPoint(4,0.);

            spring->enableSpring(5,true);
            spring->setStiffness(5,stiffness);
            spring->setDamping(5,damping);
            spring->setEquilibriumPoint(5,-angle);
        }
		
        link_prev = link;

	} 
	
}

/*
// set angular velocity of the whisker base (Nadina version, infinite whisking)
void Whisker::moveWhisker(btScalar dtheta, btVector3 headAngularVelocity){
	// OK. need to solve the problem that, this velocity is in correct frame from initialization, 
	// soon after the head rotates, the whisker itself won't rotate with the head.
	// OK. finished. Then clean up the testing mess.
	btScalar dphi = -dtheta * get_dphi(row-1);
	btScalar dzeta = dtheta * get_dzeta(row-1);
	
	if(side){ // right side
		dtheta = -dtheta;
		dphi = -dphi;
		dzeta = -dzeta;
	}

	btVector3 worldProtraction = (basepoint->getWorldTransform().getBasis()*btVector3(0,0,dtheta));
	btVector3 worldElevation = (basepoint->getWorldTransform().getBasis()*btVector3(0,dphi,0));
	btVector3 worldTorsion = (whisker[0]->getWorldTransform().getBasis()*btVector3(dzeta,0,0));
	btVector3 totalTorque = worldProtraction+worldElevation+worldTorsion;

	base->setAngularVelocity(totalTorque + headAngularVelocity);
	
}
*/

// set angular velocity of the whisker base (Yifu version, read from MATLAB output)
void Whisker::whisk(btScalar a_vel_0, btScalar a_vel_1, btScalar a_vel_2, btVector3 headAngularVelocity){
	// OK. need to solve the problem that, this velocity is in correct frame from initialization, 
	// soon after the head rotates, the whisker itself won't rotate with the head.
	// OK. finished. Then clean up the testing mess.
	// 2020/02/20: now modify this code to incorporate just changing the angular velovity of "base"
	//			   but first, get modify matlab script to just generate angular velocity

	btVector3 localAngularVelocity = btVector3(a_vel_0, a_vel_1, a_vel_2);
	base->setAngularVelocity(localAngularVelocity + headAngularVelocity);
	
}


btRigidBody* Whisker::get_unit(int idx) const{
	return whisker[idx];
}

btRigidBody* Whisker::get_base() const{
	return base;
}



// return collision status, given by collide array
std::vector<int> Whisker::getCollision(){
	std::vector<int> flags;

	for (int i=0; i<whisker.size(); i++){
		int f = collide[i];
		if(PRINT){
			std::cout << "c " << i << ": " << f << std::endl;
		}
		flags.push_back(f);
		collide[i]=0;
	}
	return flags;
}



// function to get torque at whisker base
btVector3 Whisker::getTorques() const{

	btVector3 torques = baseConstraint->getJointFeedback()->m_appliedTorqueBodyA;
	if(PRINT){
		std::cout << "Mx : " << torques[0] << std::endl;
		std::cout << "My : " << torques[1] << std::endl;
		std::cout << "Mz : " << torques[2] << std::endl;
	}
	return torques;
}

// function to get forces at whisker base
btVector3 Whisker::getForces() const{

	btVector3 forces = baseConstraint->getJointFeedback()->m_appliedForceBodyA;
	if(PRINT){
		std::cout << "Fx : " << forces[0] << std::endl;
		std::cout << "Fy : " << forces[1] << std::endl;
		std::cout << "Fz : " << forces[2] << std::endl;
	}
	return forces;
}


// function to obtain the world coordinates of each whisker unit
std::vector<btScalar> Whisker::getX() const{

	std::vector<btScalar> trajectories;
	trajectories.push_back(base->getCenterOfMassTransform().getOrigin()[0]);

	// loop through units and get world coordinates of each
	for (int i=0; i<whisker.size(); i++){
		btScalar x = whisker[i]->getCenterOfMassTransform().getOrigin()[0];
		if(PRINT){
			std::cout << "x " << i << ": " << x << std::endl;
		}
		trajectories.push_back(x);
	}
	return trajectories;
}

// function to obtain the world coordinates of each whisker unit
std::vector<btScalar> Whisker::getY() const{

	std::vector<btScalar> trajectories;
	trajectories.push_back(base->getCenterOfMassTransform().getOrigin()[1]);

	// loop through units and get world coordinates of each
	for (int i=0; i<whisker.size(); i++){
		btScalar y = whisker[i]->getCenterOfMassTransform().getOrigin()[1];
		if(PRINT){
			std::cout << "y " << i << ": " << y << std::endl;
		}
		trajectories.push_back(y);
	}
	return trajectories;
}

// function to obtain the world coordinates of each whisker unit
std::vector<btScalar> Whisker::getZ() const{

	std::vector<btScalar> trajectories;
	trajectories.push_back(base->getCenterOfMassTransform().getOrigin()[2]);

	// loop through units and get world coordinates of each
	for (int i=0; i<whisker.size(); i++){
		btScalar z = whisker[i]->getCenterOfMassTransform().getOrigin()[2];
		if(PRINT){
			std::cout << "z " << i << ": " << z << std::endl;
		}
		trajectories.push_back(z);
	}
	return trajectories;
}


btScalar Whisker::calc_base_radius(int row, int col, btScalar S) const{
	btScalar dBase = 0.041 + 0.002*S + 0.011*row - 0.0039*col;
    return (dBase/2.*1e-3) * SCALE;
}

btScalar Whisker::calc_slope(btScalar L, btScalar rbase, int row, int col) const{
    btScalar S = L/SCALE*1e3;
    btScalar rb = rbase/SCALE*1e3;
    btScalar slope = 0.0012 + 0.00017*row - 0.000066*col + 0.00011*pow(col,2);
    btScalar rtip = (rb - slope*S)/2.;
    if(rtip <= 0.0015){
        rtip = 0.0015;
    }
    slope = (rb-rtip)/S;    
    return slope;
}

btScalar Whisker::calc_mass(btScalar length, btScalar R, btScalar r, btScalar rho) const{
    btScalar m = rho*(PI*length/3)*(pow(R,2) + R*r + pow(r,2));    
    return m;
}

btScalar Whisker::calc_inertia(btScalar radius) const{
	btScalar I = 0.25*PI*pow(radius,4);		
    return I;
}

btScalar Whisker::calc_com(btScalar length, btScalar R, btScalar r) const{
    btScalar com = length/4*(pow(R,2) + 2*R*r + 3*pow(r,2))/(pow(R,2) + R*r + pow(r,2));
    return com;
}

btScalar Whisker::calc_volume(btScalar length, btScalar R, btScalar r) const{
    btScalar vol = PI*length/3*(pow(R,2) + R*r + pow(r,2));
    return vol;
}

btScalar Whisker::calc_stiffness(btScalar E, btScalar I, btScalar length) const{
    btScalar k = E*I/length;
    return k;
}


btScalar Whisker::calc_damping(btScalar k, btScalar M, btScalar CoM, btScalar zeta, btScalar dt) const{
    btScalar actual_damp = zeta * 2 * CoM * sqrt(k * M);
    btScalar offset = CoM*CoM*M/dt;
    btScalar c = dt/(offset+actual_damp);   
    return c;
}

// function to get zeta angle of whisker motion (depends on row)
float Whisker::get_dzeta(int index) const{
	return dzeta[index];
}

// function to get phi angle of whisker motion (depends on row)
float Whisker::get_dphi(int index) const{
	return dphi[index];
}

