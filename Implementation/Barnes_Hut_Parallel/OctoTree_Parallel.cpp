//
// Created by shernand on 07/06/18.
//

#include <ctime>
#include <iostream>
#include "OctoTree_Parallel.h"

OctoTree_Parallel::OctoTree_Parallel(std::vector<Particle> pParticles, double newton_g, double box_size, double timestep,
                                 double tree_thres, double softening, MPI_Comm pCommunicator)  : root(-box_size, box_size, -box_size, box_size, -box_size, box_size,"0") {
    // retrieving the number of proc and the rank in the proc pool
    MPI_Comm_rank(pCommunicator, &m_prank);
    MPI_Comm_size(pCommunicator, &m_psize);
    m_communicator = pCommunicator;
    MPI_Barrier(m_communicator);
    setParticles(pParticles);
    setNewton_g(newton_g);
    setBox_size(box_size);
    setTimestep(timestep);
    setTree_thres(tree_thres);
    setSoftening(softening);
    std::vector<double> timings;
    timings.push_back(0.0);
    timings.push_back(0.0);
    timings.push_back(0.0);
    setTimings(timings);
   // setM_communicator(communicator);

    root = Cell(-getBox_size(), getBox_size(), -getBox_size(), getBox_size(), -getBox_size(), getBox_size(),"0");
}

const std::vector<Particle> &OctoTree_Parallel::getParticles() const {
    return particles;
}

void OctoTree_Parallel::setParticles(const std::vector<Particle> &particles) {
    OctoTree_Parallel::particles = particles;
}

double OctoTree_Parallel::getNewton_g() const {
    return newton_g;
}

void OctoTree_Parallel::setNewton_g(double newton_g) {
    OctoTree_Parallel::newton_g = newton_g;
}

double OctoTree_Parallel::getBox_size() const {
    return box_size;
}

void OctoTree_Parallel::setBox_size(double box_size) {
    OctoTree_Parallel::box_size = box_size;
}

double OctoTree_Parallel::getTimestep() const {
    return timestep;
}

void OctoTree_Parallel::setTimestep(double timestep) {
    OctoTree_Parallel::timestep = timestep;
}

double OctoTree_Parallel::getTree_thres() const {
    return tree_thres;
}

void OctoTree_Parallel::setTree_thres(double tree_thres) {
    OctoTree_Parallel::tree_thres = tree_thres;
}

double OctoTree_Parallel::getSoftening() const {
    return softening;
}

void OctoTree_Parallel::setSoftening(double softening) {
    OctoTree_Parallel::softening = softening;
}

const std::vector<double> &OctoTree_Parallel::getTimings() const {
    return timings;
}

void OctoTree_Parallel::setTimings(const std::vector<double> &timings) {
    OctoTree_Parallel::timings = timings;
}

void OctoTree_Parallel::evolve() {
    //std::clock_t start,end;
    double start, end;
    //Tree Construction
    //For now it is not a parallel process
    //if (m_prank != 0) {
//    std::cout<<"P"<<m_prank<<" tree construction with "<< particles.size() << "particles"<<std::endl;
        //start = std::clock();
    start=MPI_Wtime();
//    std::cout<<"P"<<m_prank<<" BoxSize "<<getBox_size()<<std::endl;
    root = Cell(-getBox_size(), getBox_size(), -getBox_size(), getBox_size(), -getBox_size(), getBox_size(), "0");
//    std::cout<<"P"<<m_prank<<" tree construction with root "<<std::endl;
    MPI_Barrier(m_communicator);

    for(int i=0;i<particles.size();i++) {
//        std::cout<<"P"<<m_prank<<" PRE add particle "<<particles[i].getName()<<std::endl;
        root.add(particles[i]);
//        std::cout<<"P"<<m_prank<<" POST add particle "<<particles[i].getName()<<std::endl;
    }

        end = MPI_Wtime();
//    std::cout<<"Done constructing the tree at rank "<<m_prank<<std::endl;
        timings[0] = ( end - start ) / (double) CLOCKS_PER_SEC;



    //Force
    particles = root.particles();

//    std::cout<<"P"<<m_prank<<" N_particles "<<particles.size()<<std::endl;
    int n_particles_local;
//    int extra_particles;
    n_particles_local = particles.size() / m_psize;
//    std::cout<<"P"<<m_prank<<" N_particles LOCAL "<<n_particles_local<<std::endl;
//    extra_particles = particles.size() % m_psize;
//    std::cout<<"P"<<m_prank<<" extra LOCAL "<<extra_particles<<std::endl;
    int start_index, end_index;
//    if (m_prank < extra_particles){
//        start_index = (m_prank * (n_particles_local + 1));
//        end_index = (start_index + n_particles_local + 1);
//    }
//    else{
//        start_index = (m_prank * n_particles_local + extra_particles);
//        end_index = (start_index + n_particles_local);
//    }

    start_index = 0 + m_prank*n_particles_local;
    end_index = start_index + n_particles_local;

//    std::cout<<"P"<<m_prank<<" START "<<start_index<<std::endl;
//    std::cout<<"P"<<m_prank<<" END "<<end_index<<std::endl;
    std::vector<Particle> particles_local;
    std::vector<Particle>::iterator vi;
    vi = particles.begin();
    vi+=start_index;
    for(int i = 0;i<(end_index-start_index);i++,vi++){
        particles_local.push_back(*vi);
    }

    std::vector<double> x_local;
    std::vector<double> y_local;
    std::vector<double> z_local;
    std::vector<double> vx_local;
    std::vector<double> vy_local;
    std::vector<double> vz_local;
//    start = std::clock();
    start = MPI_Wtime();
    //Now it evolves but using only the local particles
    for (auto &particle: particles_local) {
//        std::cout<<"P"<<m_prank<<" is processing particle "<<particle.getName()<<std::endl;
        std::vector<Cell> cells;
        cells.push_back(root);
        while(!cells.empty()){
            Cell cell = cells.back();
            cells.pop_back();
            if(cell.meetsCriterion(particle,getTree_thres(), getSoftening())){
                if(cell.getN()>0){
                    particle.kick(cell,newton_g, getTimestep(), getSoftening());
                }
            }
            else{
                std::vector<Cell> daughterCells = cell.getDaughters();
                cells.insert(cells.end(), daughterCells.begin(), daughterCells.end());
            }
        }
        particle.drift(getTimestep());
        //Because MPI cannot manage our Particle objects
        x_local.push_back(particle.getX());
        y_local.push_back(particle.getY());
        z_local.push_back(particle.getZ());
        vx_local.push_back(particle.getVx());
        vy_local.push_back(particle.getVy());
        vz_local.push_back(particle.getVz());
    }
//    std::cout<<"P"<<m_prank<<" Finished computing the force "<<std::endl;

//    end = std::clock();
    end = MPI_Wtime();
    timings[1] += ( end - start ) / (double) CLOCKS_PER_SEC;
//    start = std::clock();
    start = MPI_Wtime();
    //Gathers how many particles each rank has processed
    int *recvcounts = NULL;

    /* Only root has the received data */
    if (m_prank == 0) {
        recvcounts = (int *)(malloc(m_psize * sizeof(int)));
    }
    //MPI_Barrier(m_communicator); //To be sure everybody has everything
    int local_size = x_local.size();
    MPI_Gather(&local_size, 1, MPI_INT,
               recvcounts, 1, MPI_INT,
               0, m_communicator);


    //Now it knows how many particles expect. Now it needs to find out the displacements
    /*
     * Figure out the total length of string,
     * and displacements for each rank
     */

    //MPI_Barrier(m_communicator); //To be sure everybody has everything

    int totlen = 0;
    int *displs = NULL;

    if (m_prank == 0) {
        displs = (int *)(malloc(m_psize * sizeof(int) ));

        displs[0] = 0;
        totlen += recvcounts[0];

        for (int i=1; i<m_psize; i++) {
            totlen += recvcounts[i];
            displs[i] = displs[i-1] + recvcounts[i-1] ;
//            std::cout<<"Recieved "<<recvcounts[i]<<std::endl;
//            std::cout<<"Displacement "<<displs[i]<<std::endl;
        }

    }

    //Now it needs to gather all the particles
    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> z;
    std::vector<double> vx;
    std::vector<double> vy;
    std::vector<double> vz;
    if (m_prank == 0) {
//        std::cout<<"Total lenght of messages is "<<totlen<<std::endl;
        x.resize(totlen);
        y.resize(totlen);
        z.resize(totlen);
        vx.resize(totlen);
        vy.resize(totlen);
        vz.resize(totlen);

    }
//    if (m_prank != 0) {
//        std::cout<<"P"<<m_prank<<" is sending "<<x_local.size()<<std::endl;
//        std::cout<<"P"<<m_prank<<" is sending "<<y_local.size()<<std::endl;
//        std::cout<<"P"<<m_prank<<" is sending "<<z_local.size()<<std::endl;
//        std::cout<<"P"<<m_prank<<" is sending "<<vx_local.size()<<std::endl;
//        std::cout<<"P"<<m_prank<<" is sending "<<vy_local.size()<<std::endl;
//        std::cout<<"P"<<m_prank<<" is sending "<<vz_local.size()<<std::endl;
//    }
    MPI_Request  request;
    MPI_Status status;

    MPI_Isend(&x_local[0], local_size, MPI_DOUBLE, 0, 0, m_communicator,&request);
    if (m_prank == 0){
        for(int i=0;i<m_psize;i++){
            MPI_Recv(&x[0+i*local_size], local_size , MPI_DOUBLE, i , 0, m_communicator, MPI_STATUS_IGNORE);
        }
    }
    MPI_Wait(&request,MPI_STATUS_IGNORE);

    MPI_Isend(&y_local[0], local_size, MPI_DOUBLE, 0, 0, m_communicator,&request);
    if (m_prank == 0){
        for(int i=0;i<m_psize;i++){
            MPI_Recv(&y[0+i*local_size], local_size , MPI_DOUBLE, i , 0, m_communicator, MPI_STATUS_IGNORE);
        }
    }
    MPI_Wait(&request,MPI_STATUS_IGNORE);

    MPI_Isend(&z_local[0], local_size, MPI_DOUBLE, 0, 0, m_communicator,&request);
    if (m_prank == 0){
        for(int i=0;i<m_psize;i++){
            MPI_Recv(&z[0+i*local_size], local_size , MPI_DOUBLE, i , 0, m_communicator, MPI_STATUS_IGNORE);
        }
    }
    MPI_Wait(&request,MPI_STATUS_IGNORE);

    MPI_Isend(&vx_local[0], local_size, MPI_DOUBLE, 0, 0, m_communicator,&request);
    if (m_prank == 0){
        for(int i=0;i<m_psize;i++){
            MPI_Recv(&vx[0+i*local_size], local_size , MPI_DOUBLE, i , 0, m_communicator, MPI_STATUS_IGNORE);
        }
    }
    MPI_Wait(&request,MPI_STATUS_IGNORE);

    MPI_Isend(&vy_local[0], local_size, MPI_DOUBLE, 0, 0, m_communicator,&request);
    if (m_prank == 0){
        for(int i=0;i<m_psize;i++){
            MPI_Recv(&vy[0+i*local_size], local_size , MPI_DOUBLE, i , 0, m_communicator, MPI_STATUS_IGNORE);
        }
    }
    MPI_Wait(&request,MPI_STATUS_IGNORE);

    MPI_Isend(&vz_local[0], local_size, MPI_DOUBLE, 0, 0, m_communicator,&request);
    if (m_prank == 0){
        for(int i=0;i<m_psize;i++){
            MPI_Recv(&vz[0+i*local_size], local_size , MPI_DOUBLE, i , 0, m_communicator, MPI_STATUS_IGNORE);
        }
    }
    MPI_Wait(&request,MPI_STATUS_IGNORE);
//
//    if (m_prank == 0) {
////        MPI_Gatherv(&x_local[0], local_size, MPI_DOUBLE, &x[0], recvcounts,
////                    displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);
//        MPI_Gather(&x_local[0], local_size, MPI_DOUBLE, &x[0],
//                    local_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
//    } else {
////        MPI_Gatherv(&x_local[0], local_size, MPI_DOUBLE, NULL, NULL ,NULL,
////                    MPI_DOUBLE, 0, MPI_COMM_WORLD);
//        MPI_Send(&x_local[0],local_size,MPI_DOUBLE,0,0,m_communicator);
//    }
//    if (m_prank == 0) {
////        MPI_Gatherv(&y_local[0], local_size, MPI_DOUBLE, &y[0], recvcounts,
////                    displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);
//        MPI_Gather(&y_local[0], local_size, MPI_DOUBLE, &y[0],
//                   local_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
//    } else {
////        MPI_Gatherv(&y_local[0], local_size, MPI_DOUBLE, NULL, NULL ,NULL,
////                    MPI_DOUBLE, 0, MPI_COMM_WORLD);
//        MPI_Send(&y_local[0],local_size,MPI_DOUBLE,0,0,m_communicator);
//    }
//    if (m_prank == 0) {
////        MPI_Gatherv(&z_local[0], local_size, MPI_DOUBLE, &z[0], recvcounts,
////                    displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);
//        MPI_Gather(&z_local[0], local_size, MPI_DOUBLE, &z[0],
//                    local_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
//    } else {
////        MPI_Gatherv(&z_local[0], local_size, MPI_DOUBLE, NULL, NULL ,NULL,
////                    MPI_DOUBLE, 0, MPI_COMM_WORLD);
//        MPI_Send(&z_local[0],local_size,MPI_DOUBLE,0,0,m_communicator);
//    }
//    MPI_Barrier(m_communicator); //To be sure everybody has everything
//    if (m_prank == 0) {
////        MPI_Gatherv(&vx_local[0], local_size, MPI_DOUBLE, &vx[0], recvcounts,
////                    displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);
//        MPI_Gather(&vx_local[0], local_size, MPI_DOUBLE, &vx[0],
//                   local_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
//    } else {
////        MPI_Gatherv(&vx_local[0], local_size, MPI_DOUBLE, NULL, NULL ,NULL,
////                    MPI_DOUBLE, 0, MPI_COMM_WORLD);
//        MPI_Send(&vx_local[0],local_size,MPI_DOUBLE,0,0,m_communicator);
//    }
//    MPI_Barrier(m_communicator); //To be sure everybody has everything
//    if (m_prank == 0) {
////        MPI_Gatherv(&vy_local[0], local_size, MPI_DOUBLE, &vy[0], recvcounts,
////                    displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);
//        MPI_Gather(&vy_local[0], local_size, MPI_DOUBLE, &vy[0],
//                    local_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
//    } else {
////        MPI_Gatherv(&vy_local[0], local_size, MPI_DOUBLE, NULL, NULL ,NULL,
////                    MPI_DOUBLE, 0, MPI_COMM_WORLD);
//        MPI_Send(&vy_local[0],local_size,MPI_DOUBLE,0,0,m_communicator);
//    }
//    MPI_Barrier(m_communicator); //To be sure everybody has everything
//    if (m_prank == 0) {
////        MPI_Gatherv(&vz_local[0], local_size, MPI_DOUBLE, &vz[0], recvcounts,
////                    displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);
//        MPI_Gather(&vz_local[0], local_size, MPI_DOUBLE, &vz[0],
//                   local_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
//    } else {
////        MPI_Gatherv(&vz_local[0], local_size, MPI_DOUBLE, NULL, NULL ,NULL,
////                    MPI_DOUBLE, 0, MPI_COMM_WORLD);
//        MPI_Send(&vz_local[0],local_size,MPI_DOUBLE,0,0,m_communicator);
//    }
//    MPI_Barrier(m_communicator); //To be sure everybody has everything

//    MPI_Gatherv(&x_local[0], x_local.size(), MPI_DOUBLE,
//                &x[0], recvcounts, displs, MPI_DOUBLE,
//                0, m_communicator);
//    //MPI_Barrier(m_communicator); //To be sure everybody has everything
//    MPI_Gatherv(&y_local[0], y_local.size(), MPI_DOUBLE,
//                &y[0], recvcounts, displs, MPI_DOUBLE,
//                0, m_communicator);
//    //MPI_Barrier(m_communicator); //To be sure everybody has everything
//    MPI_Gatherv(&z_local[0], z_local.size(), MPI_DOUBLE,
//                &z[0], recvcounts, displs, MPI_DOUBLE,
//                0, m_communicator);
//    //MPI_Barrier(m_communicator); //To be sure everybody has everything
//    MPI_Gatherv(&vx_local[0], vx_local.size(), MPI_DOUBLE,
//                &vx[0], recvcounts, displs, MPI_DOUBLE,
//                0, m_communicator);
//   // MPI_Barrier(m_communicator); //To be sure everybody has everything
//    MPI_Gatherv(&vy_local[0], vy_local.size(), MPI_DOUBLE,
//                &vy[0], recvcounts, displs, MPI_DOUBLE,
//                0, m_communicator);
//    //MPI_Barrier(m_communicator); //To be sure everybody has everything
//    MPI_Gatherv(&vz_local[0], vz_local.size(), MPI_DOUBLE,
//                &vz[0], recvcounts, displs, MPI_DOUBLE,
//                0, m_communicator);
//    //MPI_Barrier(m_communicator); //To be sure everybody has everything
//    std::cout<<"P"<<m_prank<<" sent messages "<<std::endl;




//    end = std::clock();

    if (m_prank == 0) {
//        std::cout<<"P"<<m_prank<<" root about to free the arrays "<<std::endl;
        free(displs);
        free(recvcounts);
//        std::cout<<"P"<<m_prank<<" root freed the arrays "<<std::endl;
    }

    //Recreate the particle vector
    int N_local;

    if (m_prank == 0) {
        std::vector<Particle> particles_tmp;
        for(int i=0;i<totlen;i++){
//            std::cout<<"P"<<m_prank<<" Particle "<< std::to_string(i)<<" coord : " << x[i] <<y[i]<< z[i]<< std::endl;
            Particle particle = Particle(x[i],y[i],z[i], vx[i], vy[i], vz[i], std::to_string(i));
            particles_tmp.push_back(particle);
        }
        particles=particles_tmp;
        N_local=particles.size();
    }


    MPI_Bcast(&N_local, 1, MPI_INT, 0, MPI_COMM_WORLD);
//    std::cout<<"P"<<m_prank<<" N_local "<<N_local<<std::endl;
    //MPI_Barrier(m_communicator); //To be sure everybody has everything

    if (m_prank != 0) {
        x.resize(N_local);
        y.resize(N_local);
        z.resize(N_local);
        vx.resize(N_local);
        vy.resize(N_local);
        vz.resize(N_local);
        //particles.resize(N);//Apparently not necessary
    }
    MPI_Barrier(m_communicator); //To be sure everybody has everything
    //Does the broadcoasts
    MPI_Bcast(&x[0], x.size(), MPI_DOUBLE, 0, m_communicator);
//    MPI_Barrier(m_communicator); //To be sure everybody has everything
    MPI_Bcast(&y[0], y.size(), MPI_DOUBLE, 0, m_communicator);
//    MPI_Barrier(m_communicator); //To be sure everybody has everything
    MPI_Bcast(&z[0], z.size(), MPI_DOUBLE, 0, m_communicator);
//    MPI_Barrier(m_communicator); //To be sure everybody has everything
    MPI_Bcast(&vx[0], vx.size(), MPI_DOUBLE, 0, m_communicator);
//    MPI_Barrier(m_communicator); //To be sure everybody has everything
    MPI_Bcast(&vy[0], vy.size(), MPI_DOUBLE, 0, m_communicator);
//    MPI_Barrier(m_communicator); //To be sure everybody has everything
    MPI_Bcast(&vz[0], vz.size(), MPI_DOUBLE, 0, m_communicator);
//    MPI_Barrier(m_communicator); //To be sure everybody has everything


    //Recreate the particle vector
    if (m_prank != 0) {
        std::vector<Particle> particles_tmp;
        for(int i=0;i<x.size();i++){
//            std::cout<<"P"<<m_prank<<" Particle "<< std::to_string(i)<<" coord : " << x[i] <<y[i]<< z[i]<< std::endl;
            Particle particle = Particle(x[i],y[i],z[i], vx[i], vy[i], vz[i], std::to_string(i));
            particles_tmp.push_back(particle);
        }
//        std::cout<<"P"<<m_prank<<" registers "<<x.size()<<" Particles."<<std::endl;
        particles=particles_tmp;
        particles.shrink_to_fit();
//        std::cout<<"P"<<m_prank<<" FITTED "<<particles.size()<<" Particles."<<std::endl;
    }

    x.resize(0);
    y.resize(0);
    z.resize(0);
    vx.resize(0);
    vy.resize(0);
    vz.resize(0);
    x.shrink_to_fit();
    y.shrink_to_fit();
    z.shrink_to_fit();
    vx.shrink_to_fit();
    vy.shrink_to_fit();
    vz.shrink_to_fit();
    end = MPI_Wtime();
    timings[2] += ( end - start ) / (double) CLOCKS_PER_SEC;

}

const Cell &OctoTree_Parallel::getRoot() const {
    return root;
}

void OctoTree_Parallel::setRoot(const Cell &root) {
    OctoTree_Parallel::root = root;
}

int OctoTree_Parallel::getM_prank() const {
    return m_prank;
}

void OctoTree_Parallel::setM_prank(int m_prank) {
    OctoTree_Parallel::m_prank = m_prank;
}

int OctoTree_Parallel::getM_psize() const {
    return m_psize;
}

void OctoTree_Parallel::setM_psize(int m_psize) {
    OctoTree_Parallel::m_psize = m_psize;
}

MPI_Comm OctoTree_Parallel::getM_communicator() const {
    return m_communicator;
}

void OctoTree_Parallel::setM_communicator(MPI_Comm m_communicator) {
    OctoTree_Parallel::m_communicator = m_communicator;
}
