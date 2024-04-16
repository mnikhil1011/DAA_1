#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include<vector>

using namespace std;

/// @brief width of the screen
int width=800;
/// @brief height of the screen
int height=500;
/// @brief last time we executed a step of the code
double lastUpdateTime=0;

/// @brief this is the class encapsulating the points,and its functions
class Points{
    
public:
    /// @brief stores the location of the valid points , a point is valid if it has a chance to be in the hull
    vector<Vector2>points_location;
    /// @brief stores the points which have been identified to be in the hull
    vector<Vector2>points_in_hull;

    /// @brief has the points which cant be a part of the hull
    vector<Vector2>invalid;

    /// @brief shows us all the points which have been checked in the correct iteration
    vector<Vector2>blue;

    /// @brief stores the points incase user wants to restart
    vector<Vector2>restart;

    /// @brief this checks if the points clash
    /// @param p1 point 1
    /// @param p2 point 2
    /// @return returns 1 if they clash
    bool clash(Vector2 p1,Vector2 p2)
    {
        float dx = p2.x - p1.x;
        float dy = p2.y - p1.y;
        int dist =sqrt(dx * dx + dy * dy);
        return dist<12;
    }

    ///@brief thich checks if the given point has a valid location
    ///@param point checks if this point is valid
    ///@note a point is valid if it doesnt overlap with any other point
    ///@see clash()
    bool isvalid_point(Vector2 point)
    {
        for(auto itr:points_location)
        {
            if(clash(point,itr))
            {
                return 0;
            }
        }
        return 1;
    }

    /// @brief this adds the point to points_location
    /// @param new_point the point clicked by the user
    ///@note it uses the isvalid_point() function to determine if its valid
    ///@attention if a point is clicked on the edge it shifts it so that it can fit in frame
    void add_point(Vector2 new_point)
    {
        if(new_point.y>height-50)
            return;
        
        if(new_point.x<6)
            new_point.x=6;
        else if(new_point.x>width-6)
            new_point.x=width-6;    
        if(new_point.y<6)
            new_point.y=6;
        else if(new_point.y>height-56)
            new_point.y=height-56;   

        if(isvalid_point(new_point))       
            points_location.push_back(new_point);
        
    }

    /// @brief draws the points on screen
    /// @note the points are while colour
    void draw()
    {
        for(auto itr:points_location)
        {
            DrawCircle(itr.x,itr.y,5,WHITE);
        }
        
    }
    /// @brief draws the points which have already been computed
    void draw_blue()
    {
        for(auto itr:blue)
        {
            DrawCircle(itr.x,itr.y,5,BLUE);
        }
        
    }

    /// @brief draws the lines of the hull on screen
    /// @param over this param tells us if the line on top is temporary or permanent (this is used to decide color of line)
    ///@note if the line is while it is is part of the hull
    void draw_line(bool over)
    {
        for(unsigned int i=1;i<points_in_hull.size()-1;i++)
        {
            DrawLine(points_in_hull[i-1].x,points_in_hull[i-1].y,points_in_hull[i].x,points_in_hull[i].y,WHITE);
        }
        int i=points_in_hull.size()-1;
        if(over)
        {
        
            DrawLine(points_in_hull[i-1].x,points_in_hull[i-1].y,points_in_hull[i].x,points_in_hull[i].y,WHITE);
        }
        else
        {
            DrawLine(points_in_hull[i-1].x,points_in_hull[i-1].y,points_in_hull[i].x,points_in_hull[i].y,BLACK);
        }
    }
    /// @brief finds the leftmost point 
    /// @return the index of the leftmost point
    int find_left()
    {
        int minloc=0;
        for(unsigned i=1;i<points_location.size();i++)
        {
            if(points_location[i].x<points_location[minloc].x)
            {
                minloc=i;
            }
        }
        Vector2 addnew;
        addnew.x=points_location[minloc].x;
        addnew.y=points_location[minloc].y;
        points_in_hull.push_back(addnew);
        return minloc;
    }

    /// @brief crossproduct to let us know if point is on right /left or collinear
    /// @return if its 0 rhen colinear ,if 2 then clockwise
    int orientation(Vector2 p, Vector2 q, Vector2 r) 
    {
        int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
        if (val == 0) return 0; 
        return (val > 0) ? 1 : 2; 
    }
    /// @brief calculates if i can be the next point on hull
    /// @param cur current final point on hull
    /// @param next temporary next point
    /// @param i the point to compare next with
    /// @return temporarry next point
    int calculate_next(Vector2 cur,int next,int i)
    {
        if(orientation(cur,points_location[next],points_location[i])==2)
        {
            next=i;
        }
        return next;
    }

    /// @brief draws the points which cant be a part of the hull as black
    void draw_invalid()
    {
        for(auto itr:invalid)
        {
            DrawCircle(itr.x,itr.y,5,BLACK);
        }
        
    }
    /// @brief draws the line below which the points are invalid :a point is invalid if it cant be a part of the remaining hull
    /// @param start first point on hull
    /// @param cur current latest point on hull
    ///@note it is in red colour
    void draw_invalid_line(Vector2 start,Vector2 cur)
    {
        DrawLine(start.x,start.y,cur.x,cur.y,RED);
    }

    /// @brief finds the invalid points, all points below the invalid line are invalid 
    ///@see draw_invalid_line()
    /// @param start first point on hull
    /// @param cur current latest point on hull
    void get_invalid(Vector2 start,Vector2 cur)
    {
        vector<Vector2>temp;
        for(auto i:points_location)
        {
            if(orientation(cur,start,i)==1 || orientation(cur,start,i)==0)
            {
                invalid.push_back(i);
            }
            else
            {
                temp.push_back(i);
            }
        }
        points_location=temp;
        
    }
    /// @brief adds 30 points randomly
    ///@note if any point is invalid we discard it(hence the number of points can be less than 30)
    void add_random()
    {
        for(int i=0;i<30;i++)
        {
            Vector2 random_point;
            float x= GetRandomValue(0, 800),y= GetRandomValue(0, 450);
            random_point.x=x;
            random_point.y=y;
            add_point(random_point);

        }
    }
    // @brief it adds the points present in the file input points
    //@note the file is present in the folder "input" which present in the parent directory
    // void add_from_file()
    // {
    //     FILE *file = fopen("input_points.txt", "r");
    //     if (!file)
    //     {
    //         printf("Failed to open file input_points\n");
    //         return;
    //     }
    //     float x,y;
    //     while (fscanf(file, "%f %f", &x, &y) == 2)
    //     {
    //         Vector2 temp;
    //         temp.x=x;
    //         temp.y=y;
    //         add_point(temp);
    //     }
    //     fclose(file);
    //}

};

/// @brief this function is used so that the next step of the algo is executed after some time 
/// @param interval the time to wait
/// @return if we can go ahead with the next step or not
bool EventTriggered(double interval)
{
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

/// @brief the window is made here
///@attention raylib library is used to make the window

int main()
{
    cout << "Starting the screen..." << endl;
    InitWindow(width, height, "try1");
    SetTargetFPS(60);
    Color green ={20,168,133,255};
    bool select_stage=1;
    Points points;
    int cur_point,start;
    Vector2 cur_point_locn,start_locn;
    bool over=0;
    int n,i=0;
    int next;
    float time=.1;
    while (WindowShouldClose() == false)
    {
        BeginDrawing();
        if(select_stage)
        {
            /// if user left clicks a point add it to point_location if it is a valid location
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                Vector2 mousePosition = GetMousePosition();
                
                printf("Mouse clicked at: %f, %f\n", mousePosition.x, mousePosition.y);
                points.add_point(mousePosition);
            }
            ///if user presses R then add around 30 points
            if(IsKeyPressed(KEY_R))
            {
                points.add_random();
            }
            //if user presses F then add points from the file
            // if(IsKeyPressed(KEY_F))
            // {
            //     points.add_from_file();
            // }
            /// if user presses backspace then remove the last added point 
            if(IsKeyPressed(KEY_BACKSPACE))
            {
                points.points_location.pop_back();
            }
            ///if user pressed delete then remove all points on screen
            if(IsKeyPressed(KEY_DELETE))
            {
                points.points_location={};
            }
        }
        
        ///if user clicks eneter then move to next phase, i.e run the algo now
        if (!over && points.points_location.size()>2 && IsKeyPressed(KEY_ENTER)) 
        {
            select_stage=0;
            start=points.find_left();
            points.points_in_hull.push_back(points.points_location[start]);
            start_locn=points.points_location[start];
            cur_point_locn=start_locn;
            cur_point=start;
            n=(int)points.points_location.size();
            next=(cur_point+1) % points.points_location.size();
            i=0;
            time=0.1;
            points.restart=points.points_location;
        }
        if(!select_stage && !over)
        {
            ///if user presses delete while running the algorithm then stop running the algorithm and then reset the screen
            if(IsKeyPressed(KEY_DELETE))
            {
                points.points_location={};
                points.blue={};
                points.points_in_hull={};
                points.invalid={};
                select_stage=1;
                time=0.1;
                continue;
            }
            ///user can increase speed of execution by right arrow
            if(IsKeyPressed(KEY_RIGHT))
            {
                time=0;
            }
            ///reset speed of execution to .1 by left arrow
            if(IsKeyPressed(KEY_LEFT))
            {
                time=0.1;
            }
            if (EventTriggered(time))
            {
                if(n==0)
                {
                    points.points_location=points.invalid;
                    points.points_in_hull.push_back(start_locn);
                    over=1;
                    
                }
                else if(i==n)
                {
                    cur_point=next;
                    cur_point_locn=points.points_location[cur_point];
                    next=(cur_point+1) % points.points_location.size();
                    points.get_invalid(start_locn,cur_point_locn);
                    n=points.points_location.size();
                    i=0;
                    points.blue={};
                }
                else
                {

                    if(i!=0)
                    {
                        points.points_in_hull.pop_back();
                    }
                    next=points.calculate_next(cur_point_locn,next,i);
                    DrawLine(cur_point_locn.x,cur_point_locn.y,points.points_location[i].x,points.points_location[i].y,BLUE);
                    points.blue.push_back(points.points_location[i]);
                    points.points_in_hull.push_back(points.points_location[next]);


                    i++;

                }
                
            }
        }

        ClearBackground(green);

        DrawRectangleLines(0,0,width,height-50,BLACK);
        if(select_stage==1)
        {
            if(points.points_location.size()>2)
            {
                DrawText("hit enter to start ", 20, height-45, 40, WHITE);
            }
            else
            {
                DrawText("minimum 3 points to start ", 20, height-45, 40, WHITE);
            }
        }
            
        else if(over==1)
        {
            /// if user clicks enter after the algo is over then reset the screen
            DrawText("hit enter to restart ", 20, height-45, 40, WHITE);
            if(IsKeyPressed(KEY_ENTER))
            {
                over=0;
                select_stage=1;
                points.points_in_hull={};
                points.points_location={};
                points.invalid={};
            }
            ///restart execution(re run the visualization of the algorithm with the same points)
            if(IsKeyPressed(KEY_R))
            {
                points.points_in_hull={};
                points.invalid={};

                select_stage=0;
                over=0;

                points.points_location=points.restart;

                start=points.find_left();
                points.points_in_hull.push_back(points.points_location[start]);
                start_locn=points.points_location[start];
                cur_point_locn=start_locn;
                cur_point=start;
                n=(int)points.points_location.size();
                next=(cur_point+1) % points.points_location.size();
                i=0;
                time=0.1;
                continue;

            }
            
        } 
        else
        {
            DrawText("algo visualization", 20, height-45, 40, WHITE);
        }  
        points.draw();
        points.draw_blue();
        if(!select_stage)
        {
            points.draw_line(over);
            if(!over)
            {
                points.draw_invalid();
                if(points.invalid.size()>2)
                    points.draw_invalid_line(start_locn,cur_point_locn);
            }
        }   


        EndDrawing();
    }
    CloseWindow();
    return 0;
}