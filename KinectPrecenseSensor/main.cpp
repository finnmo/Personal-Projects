/*******************************************************************************
*                                                                              *
*   PrimeSense NiTE 2.0 - Simple Skeleton Sample                               *
*   Copyright (C) 2012 PrimeSense Ltd.                                         *
*                                                                              *
*******************************************************************************/
#include <stdio.h>
#include <curl/curl.h>

#include "NiTE.h"

#include <NiteSampleUtilities.h>

#define MAX_USERS 10

int DEBUG = 0; //0 for off 1 for on
bool g_visibleUsers[MAX_USERS] = {false};
nite::SkeletonState g_skeletonStates[MAX_USERS] = {nite::SKELETON_NONE};

#define USER_MESSAGE(msg) \
        {printf("[%08llu] User #%d:\t%s\n",ts, user.getId(),msg);}

void httpPost(int message) {

  CURL *curl;
  CURLcode res;

  /* In windows, this will init the winsock stuff */
  curl_global_init(CURL_GLOBAL_ALL);

  /* get a curl handle */
  curl = curl_easy_init();
  struct curl_slist *headers=NULL;
  if(curl) {
    /* First set the URL that is about to receive our POST. This URL can
       just as well be a https:// URL if that is what should receive the
       data. */
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "Authorization: Bearer eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpc3MiOiI3NTgyMDViYzM4Yjk0NTgzOTg4M2RhNTEwNWI1YmY4NiIsImlhdCI6MTYyMzIwMzEwMiwiZXhwIjoxOTM4NTYzMTAyfQ.P3zZvV9drxmUvExZu9CdFS4tkB7KO7-gGgA7vwelWTw");
        if (message == 1) {
                curl_easy_setopt(curl, CURLOPT_URL, "http://10.1.3.178:8123/api/services/input_boolean/turn_on?api_password=finnreed");
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{\"entity_id\":\"input_boolean.bed_switch\"}");
        }else if (message == 2) {
                curl_easy_setopt(curl, CURLOPT_URL, "http://10.1.3.178:8123/api/services/input_boolean/turn_off?api_password=finnreed");
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{\"entity_id\":\"input_boolean.bed_switch\"}");
        }else if (message == 3) {
                curl_easy_setopt(curl, CURLOPT_URL, "http://10.1.3.178:8123/api/services/input_boolean/turn_on?api_password=finnreed");
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{\"entity_id\":\"input_boolean.desk_switch\"}");
        }else if (message == 4) {
                curl_easy_setopt(curl, CURLOPT_URL, "http://10.1.3.178:8123/api/services/input_boolean/turn_off?api_password=finnreed");
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{\"entity_id\":\"input_boolean.desk_switch\"}");
        }

    /* Now specify the POST data */
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    //curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{\"entity_id\":\"switch.lamp\"}");

    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    /* always cleanup */
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
}

void updateUserState(const nite::UserData& user, unsigned long long ts)
{
        if (user.isNew())
                USER_MESSAGE("New")
        else if (user.isVisible() && !g_visibleUsers[user.getId()])
                USER_MESSAGE("Visible")
        else if (!user.isVisible() && g_visibleUsers[user.getId()])
                USER_MESSAGE("Out of Scene")
        else if (user.isLost())
                USER_MESSAGE("Lost")

        g_visibleUsers[user.getId()] = user.isVisible();


        if(g_skeletonStates[user.getId()] != user.getSkeleton().getState())
        {
                switch(g_skeletonStates[user.getId()] = user.getSkeleton().getState())
                {
                case nite::SKELETON_NONE:
                        USER_MESSAGE("Stopped tracking.")
                        break;
                case nite::SKELETON_CALIBRATING:
                        USER_MESSAGE("Calibrating...")
                        break;
                case nite::SKELETON_TRACKED:
                        USER_MESSAGE("Tracking!")
                        break;
                case nite::SKELETON_CALIBRATION_ERROR_NOT_IN_POSE:
                case nite::SKELETON_CALIBRATION_ERROR_HANDS:
                case nite::SKELETON_CALIBRATION_ERROR_LEGS:
                case nite::SKELETON_CALIBRATION_ERROR_HEAD:
                case nite::SKELETON_CALIBRATION_ERROR_TORSO:
                        USER_MESSAGE("Calibration Failed... :-|")
                        break;
                }
        }
}

int main(int argc, char** argv)
{
        nite::UserTracker userTracker;
        nite::Status niteRc;

        nite::NiTE::initialize();

        float bedXMax = -200;
        float bedYMax = -100;
        float bedZMax = 2280;
        float deskXMax = -250;
        float deskYMax = -100;
        float deskZMax = 2530;

        int state1 = 0; //State 0 in room, State 1 at bed and state 2 at desk
        int oldState1 = 0; //Keep track to avoid updates
        int state2 = 0; //State 0 in room, State 1 at bed and state 2 at desk
        int oldState2 = 0; //Keep track to avoid updates
        int state3 = 0; //State 0 in room, State 1 at bed and state 2 at desk
        int oldState3 = 0; //Keep track to avoid updates


        niteRc = userTracker.create();
        if (niteRc != nite::STATUS_OK)
        {
                printf("Couldn't create user tracker\n");
                return 3;
        }
        printf("\nStart moving around to get detected...\n(PSI pose may be required for skeleton calibration, depending on the configuration)\n");

        nite::UserTrackerFrameRef userTrackerFrame;
        while (!wasKeyboardHit())
        {
                niteRc = userTracker.readFrame(&userTrackerFrame);
                if (niteRc != nite::STATUS_OK)
                {
                        printf("Get next frame failed\n");
                        continue;
                }

                const nite::Array<nite::UserData>& users = userTrackerFrame.getUsers();
                for (int i = 0; i < users.getSize(); ++i)
                {
                        const nite::UserData& user = users[i];
                        updateUserState(user,userTrackerFrame.getTimestamp());
                        if (user.isNew())
                        {
                                userTracker.startSkeletonTracking(user.getId());
                        }
                        else if (user.getSkeleton().getState() == nite::SKELETON_TRACKED)
                        {
                                const nite::SkeletonJoint& head = user.getSkeleton().getJoint(nite::JOINT_HEAD);
                                if (head.getPositionConfidence() > .5){
                                        if (DEBUG ==1){
                                                printf("%d. (%5.2f, %5.2f, %5.2f)\n", user.getId(), head.getPosition().x, head.getPosition().y, head.getPosition().z);
                                        }

                                        if(user.getId() == 1){
                                                if ((head.getPosition().z > bedZMax) && (head.getPosition().x > bedXMax) && (head.getPosition().y < bedYMax)) {
                                                        state1 = 1;
                                                }else if((head.getPosition().z > deskZMax) && (head.getPosition().x < deskXMax) && (head.getPosition().y < deskYMax)){
                                                        state1 = 2;
                                                }else{
                                                        state1 = 0;
                                                }
                                        }else if(user.getId() == 2){
                                                if ((head.getPosition().z > bedZMax) && (head.getPosition().x > bedXMax) && (head.getPosition().y < bedYMax)) {
                                                        state2 = 1;
                                                }else if((head.getPosition().z > deskZMax) && (head.getPosition().x < deskXMax) && (head.getPosition().y < deskYMax)){
                                                        state2 = 2;
                                                }else{
                                                        state2 = 0;
                                                }
                                        }else if(user.getId() == 3){
                                                if ((head.getPosition().z > bedZMax) && (head.getPosition().x > bedXMax) && (head.getPosition().y < bedYMax)) {
                                                        state3 = 1;
                                                }else if((head.getPosition().z > deskZMax) && (head.getPosition().x < deskXMax) && (head.getPosition().y < deskYMax)){
                                                        state3 = 2;
                                                }else{
                                                        state3 = 0;
                                                }
                                        }
                                }

                                if(user.getId() == 1){
                                        if (state1 != oldState1){
                                                if ((state1 == 1) && (oldState1 == 0)) {
                                                        printf("On Bed\n");
                                                        //httpPost(1);
                                                }else if ((state1 == 0) && (oldState1 == 1)) {
                                                        printf("Left Bed\n");
                                                        //httpPost(2);
                                                }else if ((state1 == 2) && (oldState1 == 0)) {
                                                        printf("At Desk\n (%d.)", user.getId());
                                                        httpPost(3);
                                                }else if ((state1 == 0) && (oldState1 == 2)) {
                                                        if((state2 != 2)&&(state3 != 2)){
                                                                printf("Left Desk\n");
                                                                httpPost(4);
                                                        }
                                                }
                                        }
                                        oldState1 = state1;

                                }
                                else if(user.getId() == 2){
                                        if (state2 != oldState2){
                                                if ((state2 == 1) && (oldState2 == 0)) {
                                                        printf("On Bed\n");
                                                        //httpPost(1);
                                                }else if ((state2 == 0) && (oldState2 == 1)) {
                                                        printf("Left Bed\n");
                                                        //httpPost(2);
                                                }else if ((state2 == 2) && (oldState2 == 0)) {
                                                        printf("At Desk\n");
                                                        httpPost(3);
                                                }else if ((state2 == 0) && (oldState2 == 2)) {
                                                        if((state1 != 2)&&(state3 != 2)){
                                                                printf("Left Desk\n");
                                                                httpPost(4);
                                                        }
                                                }
                                        }
                                        oldState2 = state2;
                                }
                                else if(user.getId() == 3){
                                        if (state3 != oldState3){
                                                if ((state3 == 1) && (oldState3 == 0)) {
                                                        printf("On Bed\n");
                                                        //httpPost(1);
                                                }else if ((state3 == 0) && (oldState3 == 1)) {
                                                        printf("Left Bed\n");
                                                        //httpPost(2);
                                                }else if ((state3 == 2) && (oldState3 == 0)) {
                                                        printf("At Desk\n");
                                                        httpPost(3);
                                                }else if ((state3 == 0) && (oldState3 == 2)) {
                                                        if((state1 != 2)&&(state2 != 2)){
                                                                printf("Left Desk\n");
                                                                httpPost(4);
                                                        }
                                                }
                                        }
                                        oldState3 = state3;
                                }

                        }
                }

        }

        nite::NiTE::shutdown();

}
