// this is for emacs file handling -*- mode: c++; indent-tabs-mode: nil -*-

// -- BEGIN LICENSE BLOCK ----------------------------------------------
// This file is part of the GPU Voxels Software Library.
//
// This program is free software licensed under the CDDL
// (COMMON DEVELOPMENT AND DISTRIBUTION LICENSE Version 1.0).
// You can find a copy of this license in LICENSE.txt in the top
// directory of the source code.
//
// © Copyright 2014 FZI Forschungszentrum Informatik, Karlsruhe, Germany
//
// -- END LICENSE BLOCK ------------------------------------------------

//----------------------------------------------------------------------
/*!\file
 *
 * \author  Andreas Hermann
 * \date    2014-11-27
 *
 * This example program demonstrates the performance gain when using an
 * tranform offset while checking for collisions, instead of transforming
 * one of the input pointclouds.
 *
 * 1. You can transform a pointcloud and insert later in a map
 * 2. The function "collideWith" allowes to set an offset.
 *
 * Since the calculation with an offset is accomplished on the gpu, we expect the latter attempt to be faster.
 *
 */
//----------------------------------------------------------------------
#include <cstdlib>
#include <signal.h>
#include <time.h>

#include <gpu_voxels/GpuVoxels.h>
#include <gpu_voxels/helpers/MetaPointCloud.h>
#include <gpu_voxels/helpers/PointcloudFileHandler.h>
#include <gpu_voxels/logging/logging_gpu_voxels.h>

using namespace gpu_voxels;

GpuVoxelsSharedPtr gvl;

void ctrlchandler(int)
{
  gvl.reset();
  exit(EXIT_SUCCESS);
}
void killhandler(int)
{
  gvl.reset();
  exit(EXIT_SUCCESS);
}




int main(int argc, char* argv[])
{
  signal(SIGINT, ctrlchandler);
  signal(SIGTERM, killhandler);

  gvl = gpu_voxels::GpuVoxels::getInstance();
  gvl->initialize(200, 200, 200, 0.01); // ==> 100 Voxels, each one is 1 mm in size so the map represents 10x10x10 centimeter

  // add maps:
  gvl->addMap(MT_PROBAB_VOXELMAP, "myObjectVoxelmap");
  gvl->addMap(MT_PROBAB_VOXELMAP, "myObjectVoxelmap1");
  gvl->addMap(MT_PROBAB_VOXELMAP, "myObjectVoxelmap2");
  gvl->addMap(MT_PROBAB_VOXELMAP, "myObjectVoxelmap3");
  gvl->addMap(MT_PROBAB_VOXELMAP, "myObjectVoxelmap4");
  gvl->addMap(MT_PROBAB_VOXELMAP, "myObjectVoxelmap5");
  gvl->addMap(MT_PROBAB_VOXELMAP, "myObjectVoxelmap6");
  gvl->addMap(MT_BITVECTOR_VOXELLIST, "myObjectVoxellist");



  // load Voxellist
  gvl->insertPointCloudFromFile("myObjectVoxellist", "/schunk_svh/f20.binvox", true,
                                eBVM_OCCUPIED, true, Vector3f(0.06, 0.05, 0.01), 1.0);
  std::cout << "List generated" << std::endl;


  // load Metapointcloud to put into Voxelmap
  std::vector<Vector3f> pc;
  file_handling::PointcloudFileHandler::Instance()->loadPointCloud(
           "/ur5_50/base.binvox", true, pc, true, Vector3f(0.1, 0.5, 0.01), 1.0);
  std::cout << "Num points: " << pc.size() << std::endl;
  std::vector< std::vector<Vector3f> > vpc;
  vpc.push_back(pc);
  MetaPointCloud mpc(vpc);

  std::vector<Vector3f> pc1;
  file_handling::PointcloudFileHandler::Instance()->loadPointCloud(
           "/ur5_50/upperarm.binvox", true, pc1, true, Vector3f(0.2, 0.4, 0.11), 1.0);
  std::cout << "Num points: " << pc1.size() << std::endl;
  std::vector< std::vector<Vector3f> > vpc1;
  vpc1.push_back(pc1);
  MetaPointCloud mpc1(vpc1);

  std::vector<Vector3f> pc2;
  file_handling::PointcloudFileHandler::Instance()->loadPointCloud(
           "/ur5_50/forearm.binvox", true, pc2, true, Vector3f(0.3, 0.3, 0.2), 1.0);
  std::cout << "Num points: " << pc2.size() << std::endl;
  std::vector< std::vector<Vector3f> > vpc2;
  vpc2.push_back(pc2);
  MetaPointCloud mpc2(vpc2);

  std::vector<Vector3f> pc3;
  file_handling::PointcloudFileHandler::Instance()->loadPointCloud(
           "/ur5_50/shoulder.binvox", true, pc3, true, Vector3f(0.4, 0.6, 0.2), 1.0);
  std::cout << "Num points: " << pc3.size() << std::endl;
  std::vector< std::vector<Vector3f> > vpc3;
  vpc3.push_back(pc3);
  MetaPointCloud mpc3(vpc3);

  std::vector<Vector3f> pc4;
  file_handling::PointcloudFileHandler::Instance()->loadPointCloud(
           "/ur5_50/wrist1.binvox", true, pc4, true, Vector3f(0.5, 0.5, 0.3), 1.0);
  std::cout << "Num points: " << pc4.size() << std::endl;
  std::vector< std::vector<Vector3f> > vpc4;
  vpc4.push_back(pc4);
  MetaPointCloud mpc4(vpc4);

  std::vector<Vector3f> pc5;
  file_handling::PointcloudFileHandler::Instance()->loadPointCloud(
           "/ur5_50/wrist2.binvox", true, pc5, true, Vector3f(0.6, 0.4, 0.2), 1.0);
  std::cout << "Num points: " << pc5.size() << std::endl;
  std::vector< std::vector<Vector3f> > vpc5;
  vpc5.push_back(pc5);
  MetaPointCloud mpc5(vpc5);



  size_t colls;

  //----- insert map 100 times, transform, add to map and collide
  Matrix4f trans = Matrix4f::createFromRotationAndTranslation(
        Matrix3f::createIdentity(), Vector3f(0.0005, 0, 0));
  clock_t clock_tr_begin = clock();
  clock_t clock_o_begin_1;
  clock_t clock_o_begin_2;
  clock_t clock_o_begin_3;
  clock_t clock_o_begin_4;


  gvl->getMap("myObjectVoxelmap")->clearMap();
  gvl->getMap("myObjectVoxelmap1")->clearMap();
  gvl->getMap("myObjectVoxelmap2")->clearMap();
  gvl->getMap("myObjectVoxelmap3")->clearMap();
  gvl->getMap("myObjectVoxelmap4")->clearMap();
  gvl->getMap("myObjectVoxelmap5")->clearMap();
  gvl->getMap("myObjectVoxelmap6")->clearMap();
  clock_o_begin_1 = clock();
  mpc.transformSelf(&trans);
  mpc1.transformSelf(&trans);
  mpc2.transformSelf(&trans);
  mpc3.transformSelf(&trans);
  mpc4.transformSelf(&trans);
  mpc5.transformSelf(&trans);
  clock_o_begin_2 = clock();
  gvl->getMap("myObjectVoxelmap6")->insertMetaPointCloud(mpc, eBVM_SWEPT_VOLUME_START);
  gvl->getMap("myObjectVoxelmap6")->insertMetaPointCloud(mpc1, eBVM_SWEPT_VOLUME_START);
  gvl->getMap("myObjectVoxelmap6")->insertMetaPointCloud(mpc2, eBVM_SWEPT_VOLUME_START);
  gvl->getMap("myObjectVoxelmap6")->insertMetaPointCloud(mpc3, eBVM_SWEPT_VOLUME_START);
  gvl->getMap("myObjectVoxelmap6")->insertMetaPointCloud(mpc4, eBVM_SWEPT_VOLUME_START);
  gvl->getMap("myObjectVoxelmap6")->insertMetaPointCloud(mpc5, eBVM_SWEPT_VOLUME_START);
  clock_o_begin_3 = clock();
  colls = gvl->getMap("myObjectVoxellist")->as<voxellist::BitVectorVoxelList>()->collideWith(gvl->getMap("myObjectVoxelmap")->as<voxelmap::ProbVoxelMap>(), 0.1);
  clock_o_begin_4 = clock();
  clock_t clock_tr_end = clock();



  // generate the same test setup for the second test
  // gvl->getMap("myObjectVoxelmap")->clearMap();
  std::cout << "-------------------------------------" << std::endl;



  //----- insert map 100 times, calculate collision with offset
  Vector3i offset;
  clock_t clock_o_begin = clock();
  // for (size_t i = 0; i < 100; i++)
  // {
  //   gvl->getMap("myObjectVoxelmap")->clearMap();

  //   offset.x = i;
  //   gvl->getMap("myObjectVoxelmap")->insertMetaPointCloud(mpc, eBVM_SWEPT_VOLUME_START);
  //   colls = gvl->getMap("myObjectVoxellist")->as<voxellist::BitVectorVoxelList>()->collideWith(gvl->getMap("myObjectVoxelmap")->as<voxelmap::ProbVoxelMap>(), 0.1, offset);
  //   std::cout << "Collide with offset:\n colls in step " << i << " = " << colls << std::endl;
  // }
  clock_t clock_o_end = clock();




  // compare timings
  double time_transform = (double) (clock_tr_end - clock_tr_begin) / CLOCKS_PER_SEC;;
  double time_transform_1 = (double) (clock_o_begin_2 - clock_o_begin_1) / CLOCKS_PER_SEC;
  double time_transform_2 = (double) (clock_o_begin_3 - clock_o_begin_2) / CLOCKS_PER_SEC;
  double time_transform_3 = (double) (clock_o_begin_4 - clock_o_begin_3) / CLOCKS_PER_SEC;
  double time_offset = (double) (clock_o_end - clock_o_begin) / CLOCKS_PER_SEC;


  while(true)
  {
    gvl->visualizeMap("myObjectVoxelmap6");
    // gvl->visualizeMap("myObjectVoxelmap1");
    // gvl->visualizeMap("myObjectVoxelmap2");
    // gvl->visualizeMap("myObjectVoxelmap3");
    // gvl->visualizeMap("myObjectVoxelmap4");
    // gvl->visualizeMap("myObjectVoxelmap5");
    // gvl->visualizeMap("myObjectVoxellist");
    usleep(500000);
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "time with transform: " << time_transform << "\ntime with offset: " << time_offset << std::endl;
    std::cout << time_transform_1 << ", " << time_transform_2 << ", " << time_transform_3 << std::endl;
    std::cout << "-------------------------------------" << std::endl;

  }

}
