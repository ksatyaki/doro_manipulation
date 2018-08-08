//
// Created by julian on 05.08.18.
//

#include <jaco_manipulation/client/jaco_manipulation_client.h>
#include <jaco_manipulation/client/goals/objects/grasp_goal.h>
#include <jaco_manipulation/client/goals/objects/drop_goal.h>
#include <tf/tf.h>

using namespace jaco_manipulation::client;

JacoManipulationClient::JacoManipulationClient() : client_("plan_and_move_arm", true) {
  client_.waitForServer();
}

void JacoManipulationClient::moveTo(const std::string &moveit_goal, const std::string &description) {
  goals::MoveItGoal goal(moveit_goal, description);
  execute(goal);
}

void JacoManipulationClient::moveTo(const geometry_msgs::PoseStamped &pose_goal, const std::string &description) {
  goals::PoseGoal goal(pose_goal, description);
  execute(goal);
}

void JacoManipulationClient::moveTo(const sensor_msgs::JointState &joint_goal, const std::string &description) {
  goals::JointGoal goal(joint_goal.position, description);
  execute(goal);
}

void JacoManipulationClient::graspAt(const goals::kinect_goal_definitions::LimitedPose &grasp_pose_goal,
                                     const std::string &description) {
  goals::objects::GraspGoal goal(grasp_pose_goal, description);
  execute(goal);
}


void JacoManipulationClient::graspAt(goals::kinect_goal_definitions::BoundingBox &bounding_box_goal,
                                     const std::string &description) {
  bounding_box_goal.type = "grasp_bounding_box";
  goals::objects::GraspGoal goal(bounding_box_goal, description);
  execute(goal);
}

void JacoManipulationClient::dropAt(const goals::kinect_goal_definitions::LimitedPose &drop_pose_goal,
                                    const std::string &description) {
  goals::objects::DropGoal goal(drop_pose_goal, description);
  execute(goal);
}

void JacoManipulationClient::dropAt(goals::kinect_goal_definitions::BoundingBox &bounding_box_goal,
                                    const std::string &description) {
  bounding_box_goal.type = "drop_bounding_box";
  goals::objects::DropGoal goal(bounding_box_goal, description);
  execute(goal);
}

void JacoManipulationClient::execute(const goals::Goal &goal_wrapper) {
  const auto& goal = goal_wrapper.goal();

  client_.sendGoal(goal);
  client_.waitForResult();

  if (client_.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
    ROS_SUCCESS("Status  : Move to " << goal_wrapper.info() << " succeeded.");
  } else {
    ROS_ERROR_STREAM("Status  : Move to " << goal_wrapper.info() << " failed.");
  }
  std::cout << "\n";
}
