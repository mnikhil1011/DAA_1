#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;
/// @brief width of the screen
int width = 800;
/// @brief height of the screen
int height = 500;
/// @brief the last time we ran a step in the algorithm
float lastUpdateTime = 0;

/// @brief a class which encapsulates all the functions required for the points
///
///it includes functions to handle collisions of the points and to check if a point location is valid
class Points
{

public:
    /// @brief stores the locations of the points
    vector<Vector2> points_location;

    /// @brief this checks if the points clash
    /// @param p1 point 1
    /// @param p2 point 2
    /// @return returns 1 if they clash
    bool clash(Vector2 p1, Vector2 p2)
    {
        float dx = p2.x - p1.x;
        float dy = p2.y - p1.y;
        int dist = sqrt(dx * dx + dy * dy);
        return dist < 12;
    }

    ///@brief thich checks if the given point has a valid location
    ///@param point checks if this point is valid
    ///@note a point is valid if it doesnt overlap with any other point
    ///@see clash()
    bool isvalid_point(Vector2 point)
    {
        for (auto itr : points_location)
        {
            if (clash(point, itr))
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
        if (new_point.y > height - 50) // out of screen
            return;

        // move the point slightly inside to fit
        if (new_point.x < 6)
            new_point.x = 6;
        else if (new_point.x > width - 6)
            new_point.x = width - 6;
        if (new_point.y < 6)
            new_point.y = 6;
        else if (new_point.y > height - 56)
            new_point.y = height - 56;

        if (isvalid_point(new_point))
            points_location.push_back(new_point);
    }

    /// @brief draws the points on screen
    /// @note the points are while colour
    void draw()
    {
        for (auto itr : points_location)
        {
            DrawCircle(itr.x, itr.y, 5, WHITE);
        }
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

};

/// @brief this function is used so that the next step of the algo is executed after some time 
/// @param interval the time to wait
/// @return if we can go ahead with the next step or not
bool EventTriggered(float interval)
{
    float currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

/// @brief finds a pivot element to perform sorting based on pivot , it is used in finding the median in median of medians
///@note finds a pivot for an array of ** points**
/// @return returns pivot
Vector2 findPivot(vector<Vector2> arr, int left, int right)
{
    int n = right - left + 1;
    if(right==-1)
    {
        Vector2 temp;
        temp.x=-1;
        temp.y=-1;
        return temp;
    }
    if (n <= 5)
    {
        sort(arr.begin() + left, arr.begin() + right + 1, [](const Vector2 &a, const Vector2 &b)
             { return a.x < b.x; });
        return arr[left + n / 2];
    }

    int numMedians = (n + 4) / 5;
    vector<Vector2> medians;
    for (int i = 0; i < numMedians; i++)
    {
        int start = left + 5 * i;
        int end = min(left + 5 * i + 4, right);
        sort(arr.begin() + start, arr.begin() + end + 1, [](const Vector2 &a, const Vector2 &b)
             { return a.x < b.x; });
        medians.push_back(arr[start + (end - start) / 2]);
    }
    return findPivot(medians, 0, numMedians - 1);
}

/// @brief finds median of a given array
///
///it finds the median using median of medians algorithm
///@attention it finds the median in O(n) time
///@note this function finds the median of an array consisting of **points**
/// @param arr array of points
/// @return median
Vector2 find_median(vector<Vector2> arr)
{
    int need_index = ((int)arr.size() - 1) / 2;
    int left = 0;
    int right = (int)arr.size() - 1;
    if (arr.size() == 1)
        return arr[0];

    while (left <= right)
    {
        Vector2 pivot_value = findPivot(arr, left, right);
        int pivot = left;
        if(pivot_value.x!=-1)
        {
            for (int i = left; i <= right; i++)
            {
                if (arr[i].x == pivot_value.x)
                {
                    pivot = i;
                    break;
                }
            }
        }
        if(pivot_value.x==-1)
        {
            return arr[0];
        }
        
        swap(arr[pivot], arr[left]);

        int i = left + 1;
        int j = right;
        if (i >= j)
        {
            return arr[left];
        }
        while (i < j)
        {
            while (i < j && arr[i].x <= arr[left].x)
            {
                i++;
            }
            while (j > i && arr[j].x > arr[left].x)
            {
                j--;
            }
            if (i < j)
            {
                swap(arr[j], arr[i]);
                j--;
                i++;
            }
        }
        swap(arr[j], arr[left]);
        if (j == need_index)
        {
            return arr[j];
        }
        else
        {
            if (j > need_index)
            {
                right = j - 1;
            }
            else
            {
                left = j + 1;
            }
        }
        if (left >= right)
        {
            return arr[j];
        }
    }
    return arr[0];
}

/// @brief finds a pivot element to perform sorting based on pivot , it is used in finding the median in median of medians
///@note finds a pivot for an array of ** float values**
/// @return returns pivot
float findPivot_slope(vector<float> arr, int left, int right)
{
    int n = right - left + 1;
    // if(left>right)
    // {
    //     return arr[left];
    // }
    // if (n == 1)
    //     return arr[left];
    if (right == -1)
    {
        return 0;
    }
    if (n <= 5)
    {
        sort(arr.begin() + left, arr.begin() + right + 1);
        // cout<<n<<left;
        return arr[left + n / 2];
    }

    int numMedians = (n + 4) / 5;
    vector<float> medians;
    // float temp=
    for (int i = 0; i < numMedians; i++)
    {
        int start = left + 5 * i;
        int end = min(start + 4, right);
        sort(arr.begin() + start, arr.begin() + end + 1);
        medians.push_back(arr[start + (end - start) / 2]);
    }

    return findPivot_slope(medians, 0, numMedians - 1);
}

/// @brief finds median of a given array
///
///it finds the median using median of medians algorithm
///@attention it finds the median in O(n) time
///@note this function finds the median of an array consisting of **float**
/// @param arr array of slopes
/// @return median
float find_median_slope(vector<float> arr)
{
    int need_index = (int)arr.size() / 2;
    int left = 0;
    int right = (int)arr.size() - 1;
    if (arr.size() == 1)
        return arr[0];
    while (left <= right)
    {
        float pivot_value = findPivot_slope(arr, left, right);

        int pivot = left;
        if (pivot_value != 0)
        {
            for (int i = left; i <= right; i++)
            {
                if (arr[i] == pivot_value)
                {
                    pivot = i;
                    break;
                }
            }
        }
        // if(pivot_value==0)
        // {
        //     return arr[0];
        // }

        swap(arr[pivot], arr[left]);

        int i = left + 1;
        int j = right;
        if (i >= j)
        {
            return arr[left];
        }
        while (i < j)
        {
            while (i < j && arr[i] <= arr[left])
            {
                i++;
            }
            while (j > i && arr[j] > arr[left])
            {
                j--;
            }
            if (i < j)
            {
                swap(arr[j], arr[i]);
                j--;
                i++;
            }
        }
        swap(arr[j], arr[left]);
        if (j == need_index)
        {
            return arr[j];
        }
        else
        {
            if (j > need_index)
            {
                right = j - 1;
            }
            else
            {
                left = j + 1;
            }
        }
    }
    return arr[0];
}

/// @brief it stores the state for the next bridge
///
/// we recursively find the left and right sub problems
/// since we wantg to visualize it step by step instead of using a recursion stack we have used a deque
/// it would be the same because we store the state of the left and right subproblem in the deque and then we pop it and then use that info to solve the subproblem
/// this struct holds the info we need for the sub problem
struct info
{
    /// @brief stores the location of the points
    vector<Vector2> points;
    /// @brief left boundary
    Vector2 left;
    /// @brief right boundary
    Vector2 right;
};

/// @brief encapsulates all the functions and attributed needed for the upper hull
class Upper_hull
{
public:
    /// @brief left most point in the upper hull
    Vector2 xmin;
    /// @brief right most point in the upper hull
    Vector2 xmax;
    /// @brief stores all the edges in the upper hull
    vector<pair<Vector2, Vector2>> upper_edges;
    /// @brief stores the current median
    ///
    ///this is needed for visualization we display this line
    Vector2 curr_median;
    /// @brief used to store the subproblems states
    deque<info> s;

    /// @brief finds the leftmost point in the upper hull
    /// @param points all the points 
    /// @return left most point
    Vector2 find_xmin(vector<Vector2> &points)
    {
        xmin = points[0];
        for (size_t i = 1; i < points.size(); i++)
        {
            ///if we find a point with lesser x axis change xmin
            if (xmin.x > points[i].x)
            {
                xmin = points[i];
            }
            ///if same xmin choose the point with a lower y axis(in raylib origin is top right)
            else if (xmin.x == points[i].x)
            {
                if (xmin.y > points[i].y)
                {
                    xmin = points[i];
                }
            }
        }
        return xmin;
    }
    /// @brief finds the rightmost point in the upper hull
    /// @param points all the points
    /// @return the rightmost point
    Vector2 find_xmax(vector<Vector2> &points)
    {
        xmax = points[0];
        for (size_t i = 1; i < points.size(); i++)
        {
            ///if we find a point with higher x axis change xmax
            if (xmax.x < points[i].x)
            {
                xmax = points[i];
            }
            ///if same xmax choose the point with a lower y axis(in raylib origin is top right)
            else if (xmax.x == points[i].x)
            {
                if (xmax.y > points[i].y)
                {
                    xmax = points[i];
                }
            }
        }
        return xmax;
    }
    /// @brief crossproduct to let us know if point is on right /left or collinear
    /// @return if its 0 rhen colinear ,if 2 then clockwise
    int orientation(Vector2 p, Vector2 q, Vector2 r)
    {
        float val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
        if (val == 0)
            return 0;             // Collinear
        return (val > 0) ? 1 : 2; // Clockwise or Counterclockwise
    }
    /// @brief finds the upper bridge(an edge in the upper hull which passes through the median)
    /// @param points all points which can be part of the bridge
    /// @param median the median through which we want the bridge to pass
    /// @return the upper bridge
    pair<Vector2, Vector2> find_edge(vector<Vector2> points, Vector2 median)
    {
        ///edge case:it has been used for whenever an error might happen and we somehow end up with less than 2 valid points then we return an edge {median,median} ie an empty edge
        // if (points.size() < 2)
        // {
        //     // median.x-=1;
        //     return {median, median};
        // }
        ///if only 2 valid points remain then it is part of the upper bridge , hence return this edge
        // sort(points.begin(), points.end(), [](const Vector2 &a, const Vector2 &b)
        //      { return a.x < b.x; });
        if (points.size() == 2)
        {
            
            if (points[0].x < points[1].x)
                return {points[0], points[1]};
            return {points[1], points[0]};
        }

        vector<pair<Vector2, Vector2>> pairs;
        vector<Vector2> candidates;
        int n = (int)points.size();
        /// if odd number of points , add one point to candidate so that we are left with an even number of points
        if (n % 2 == 1)
        {
            candidates.push_back(points[0]);
            for (int i = 1; i < n ; i += 2)
            {
                if (points[i].x < points[i + 1].x)
                    pairs.push_back({points[i], points[i + 1]});
                else
                    pairs.push_back({points[i + 1], points[i]});
            }
        }
        ///choose pairs of 2 points; make it an ordered pair, x1<x2
        else
        {
            for (int i = 0; i < n; i += 2)
            {
                if (points[i].x < points[i + 1].x)
                    pairs.push_back({points[i], points[i + 1]});
                else
                    pairs.push_back({points[i + 1], points[i]});
            }
        }

        int m = n / 2;
        vector<float> slopes;
        ///find the median of the slopes by calculating all slopes and then performing medians of medians
        for (int i = 0; i < m; i++)
        {
            ///if parallel to the y axis the add the upper point to candidates
            if (pairs[i].first.x == pairs[i].second.x)
            {
                if (pairs[i].first.y < pairs[i].second.y)
                {
                    candidates.push_back(pairs[i].first);
                }
                else
                {
                    candidates.push_back(pairs[i].second);
                }
            }
            else
            {
                float temp = ((float)(-pairs[i].second.y + pairs[i].first.y)) / ((float)(pairs[i].second.x - pairs[i].first.x));
                slopes.push_back(temp);
            }
        }
        float median_slope;
        if (slopes.size())
            median_slope = find_median_slope(slopes);
        else
        {
            /// if no slope exists then call the function again with possible candidates
            find_edge(candidates, median);
        }

        ///seperate the pairs in small large and equal , by compairing their slope with the median slope
        vector<pair<Vector2, Vector2>> small;
        vector<pair<Vector2, Vector2>> equal;
        vector<pair<Vector2, Vector2>> large;
        for (int i = 0; i < m; i++)
        {
            if (pairs[i].first.x == pairs[i].second.x)
                continue;
            float temp = ((float)(-pairs[i].second.y + pairs[i].first.y)) / ((float)(pairs[i].second.x - pairs[i].first.x));
            float diff = (temp - median_slope);
            // if (diff < 0)
            //     diff = -diff;
            if (diff == 0)
            {
                equal.push_back(pairs[i]);
            }
            else if (temp > median_slope)
            {
                large.push_back(pairs[i]);
            }
            else
            {
                small.push_back(pairs[i]);
            }
        }
        /// find the maximum c intercept of all points with median slope
        float max_c = INT_MIN;
        for (int i = 0; i < n; i++)
        {
            float cur_c = -points[i].y - median_slope * points[i].x;
            max_c = max(max_c, cur_c);
        }
        // cout << max_c << endl;
        Vector2 pmin;
        Vector2 pmax;
        pmin.x = INT_MAX;
        pmax.x = INT_MIN;
        /// find leftmost and rightmost point with that intercept
        for (int i = 0; i < n; i++)
        {
            float cur_c = -points[i].y - median_slope * points[i].x;
            float diff = (cur_c - max_c);
            if (diff < 0)
                diff = -diff;
            if (diff <= 0.0001)
            {
                if (points[i].x > pmax.x)
                {
                    pmax = points[i];
                }
                if (points[i].x < pmin.x)
                {
                    pmin = points[i];
                }
            }
        }
        ///if they lie on the opposite sides of the median this is the bridge , hence return the edge formed
        if (pmin.x <= median.x && pmax.x > median.x)
        {
            return {pmin, pmax};
        }
        // if (pmin.x < median.x && pmax.x >= median.x)
        // {
        //     return {pmin, pmax};
        // }
        // if(pmin.x == pmax.x && pmin.x==median.x)
        // {
        //     return {pmin,pmin};
        // }
        ///if pmax is less than the median
        ///add the point with higher x coordinate tocandidates if the slope of the pair is equal or larger than the median slope
        /// add both points if the slope is smaller than the median slope
        else if (pmax.x <= median.x)
        {
            for (int i = 0; i < (int)equal.size(); i++)
            {
                Vector2 pt = equal[i].second;
                candidates.push_back(pt);
            }
            for (int i = 0; i < (int)large.size(); i++)
            {
                Vector2 pt = large[i].second;
                candidates.push_back(pt);
            }
            for (int i = 0; i < (int)small.size(); i++)
            {
                Vector2 pt1 = small[i].first;
                candidates.push_back(pt1);
                Vector2 pt2 = small[i].second;
                candidates.push_back(pt2);
            }
            
            return find_edge(candidates, median);
        }
        ///if min is greater than the median
        ///add the point with lower x coordinate tocandidates if the slope of the pair is equal or smaller than the median slope
        /// add both points if the slope is larger than the median slope
        else //if(pmin.x>median.x)
        {
            for (int i = 0; i < (int)equal.size(); i++)
            {
                Vector2 pt = equal[i].first;
                candidates.push_back(pt);
            }
            for (int i = 0; i < (int)large.size(); i++)
            {

                Vector2 pt1 = large[i].first;
                candidates.push_back(pt1);
                Vector2 pt2 = large[i].second;
                candidates.push_back(pt2);
            }
            for (int i = 0; i < (int)small.size(); i++)
            {
                Vector2 pt = small[i].first;
                candidates.push_back(pt);
            }
            return find_edge(candidates, median);
        }
        /// recursively call the function with points =  candidates
    }
    /// @brief function to calculate the upper hull
    ///normally it recursively calls itself to visualize this instead of using a recursion stack we used a deque to store the states and then call the function with that state
    /// @param points points which can be part of the hull
    /// @param left left bound of the subproblem
    /// @param right right bound of the subproblem
    void find_hull(vector<Vector2> points, Vector2 left, Vector2 right)
    {
        ///terminating crondition: handles cases when we end up with left >right then there is no subproblem to solve we return in this case
        if (left.x >= right.x)
            return;
        Vector2 median;
        /// if number of points less then 2 , then no edge exists
        if (points.size() == 0 || points.size() == 1)
            return;
        
        if (points.size() == 2)
        {
            if (points[0].x < points[1].x)
            {
                median = points[0];
            }
            else
            {
                median = points[1];
            }
        }
        else
        {
            median = find_median(points);
        }
        int flag=0;
        for(auto itr:points)
        {
            if(itr.x>median.x)
            {
                flag=1;
                break;
            }
        }
        if(flag==0)
        {
            for(auto itr:points)
            {
                if(itr.x<median.x)
                {
                    median=itr;
                    break;
                }
            }
        }
        curr_median = median;
        /// find upper bridge by calling find_edge() function
        pair<Vector2, Vector2> edge = find_edge(points, median);
        if(edge.first.x<=median.x && edge.second.x>median.x)
            upper_edges.push_back(edge);
        // if(edge.first.x>median.x || edge.second.x<median.x)
        // {
            
        // }
        // if(edge.first.x<left.x || edge.second.x>right.x)
        //     return;

            // upper_edges.push_back(edge);
        /// if points.size() ==2 then this is the last edge and there is no subproblem left
        if (points.size() == 2)
            return;
        /// store state for the next subproblem
        if (edge.second.x != right.x)
        {
            struct info info_temp;
            info_temp.left = edge.second;
            info_temp.right = right;
            info_temp.points = points;
            s.push_back(info_temp);
        }
        if (edge.first.x != left.x)
        {
            struct info info_temp;
            info_temp.left = left;
            info_temp.right = edge.first;
            info_temp.points = points;
            s.push_back(info_temp);
        }
    }
    /// @brief this function calls the upper hull function
    ///
    ///it reduces the number of valid points which can be part of the upper hull
    void find_hull_helper(vector<Vector2> &points, Vector2 left, Vector2 right)
    {
        vector<Vector2> valid;
        /// all the points present above the left right line can be part of the hull
        for (auto i : points)
        {
            if (orientation(left, right, i) == 1||orientation(left, right, i) == 0  )
            {
                valid.push_back(i);
            }
            
        }
        if (valid.size() == 0)
            return;
        find_hull(valid, left, right);
    }
    /// @brief draws the edges present in the upper hull
    void draw_upper()
    {
        for (auto i : upper_edges)
        {
            DrawLine(i.first.x, i.first.y, i.second.x, i.second.y, WHITE);
        }
    }
};

/// @brief encapsulates all the functions and attributed needed for the lower hull
class Lower_hull
{
public:
    /// @brief left most point in the lower hull
    Vector2 xmin;
    /// @brief right most point in the lower hull
    Vector2 xmax;
     /// @brief stores all the edges in the lower hull
    vector<pair<Vector2, Vector2>> lower_edges;

    /// @brief stores the current median
    ///
    ///this is needed for visualization we display this line
    Vector2 curr_median;
    /// @brief used to store the subproblems states
    deque<info> s;
    /// @brief finds the leftmost point in the lower hull
    /// @param points all the points 
    /// @return left most point
    Vector2 find_xmin(vector<Vector2> &points)
    {
        xmin = points[0];
        ///if we find a point with lesser x axis change xmin
        for (size_t i = 1; i < points.size(); i++)
        {
            if (xmin.x > points[i].x)
            {
                xmin = points[i];
            }
            ///if same xmin choose the point with a higher y axis(in raylib origin is top right)
            else if (xmin.x == points[i].x)
            {
                if (xmin.y < points[i].y)
                {
                    xmin = points[i];
                }
            }
        }
        return xmin;
    }
    /// @brief finds the rightmost point in the lower hull
    /// @param points all the points 
    /// @return right most point
    Vector2 find_xmax(vector<Vector2> &points)
    {
        xmax = points[0];
        ///if we find a point with lesser x axis change xmin
        for (size_t i = 1; i < points.size(); i++)
        {
            if (xmax.x < points[i].x)
            {
                xmax = points[i];
            }
            ///if same xmin choose the point with a higher y axis(in raylib origin is top right)
            else if (xmax.x == points[i].x)
            {
                if (xmax.y < points[i].y)
                {
                    xmax = points[i];
                }
            }
        }
        return xmax;
    }
    
    /// @brief crossproduct to let us know if point is on right /left or collinear
    /// @return if its 0 rhen colinear ,if 2 then clockwise
    int orientation(Vector2 p, Vector2 q, Vector2 r)
    {
        float val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
        if (val == 0)
            return 0;             // Collinear
        return (val > 0) ? 1 : 2; // Clockwise or Counterclockwise
    }

    /// @brief finds the upper bridge(an edge in the upper hull which passes through the median)
    /// @param points all points which can be part of the bridge
    /// @param median the median through which we want the bridge to pass
    /// @return the upper bridge
    pair<Vector2, Vector2> find_edge(vector<Vector2> points, Vector2 median)
    {
        ///edge case:it has been used for whenever an error might happen and we somehow end up with less than 2 valid points then we return an edge {median,median} ie an empty edge
        if (points.size() < 2)
        {
            // median.x-=1;
            return {median, median};
        }

        ///if only 2 valid points remain then it is part of the upper bridge , hence return this edge
        // sort(points.begin(), points.end(), [](const Vector2 &a, const Vector2 &b)
        //      { return a.x < b.x; });
        if (points.size() == 2)
        {
            
            if (points[0].x < points[1].x)
                return {points[0], points[1]};
            return {points[1], points[0]};
        }

        vector<pair<Vector2, Vector2>> pairs;
        vector<Vector2> candidates;
        int n = (int)points.size();
        /// if odd number of points , add one point to candidate so that we are left with an even number of points
        if (n % 2 == 1)
        {
            candidates.push_back(points[0]);
            for (int i = 1; i < n; i += 2)
            {
                if (points[i].x < points[i + 1].x)
                    pairs.push_back({points[i], points[i + 1]});
                else
                    pairs.push_back({points[i + 1], points[i]});
            }
        }
        ///choose pairs of 2 points; make it an ordered pair, x1<x2
        else
        {
            for (int i = 0; i < n; i += 2)
            {
                if (points[i].x < points[i + 1].x)
                    pairs.push_back({points[i], points[i + 1]});
                else
                    pairs.push_back({points[i + 1], points[i]});
            }
        }

        int m = n / 2;
        vector<float> slopes;
        ///find the median of the slopes by calculating all slopes and then performing medians of medians
        for (int i = 0; i < m; i++)
        {
            ///if parallel to the y axis the add the lower point to candidates
            if (pairs[i].first.x == pairs[i].second.x)
            {

                if (pairs[i].first.y > pairs[i].second.y)
                {

                    candidates.push_back(pairs[i].first);
                }
                else
                {
                    candidates.push_back(pairs[i].second);
                }
            }
            else
            {
                float temp = ((float)(-pairs[i].second.y + pairs[i].first.y)) / ((float)(pairs[i].second.x - pairs[i].first.x));
                slopes.push_back(temp);
            }
        }
        float median_slope;
        // median_slope = find_median_slope(slopes);
        if (slopes.size())
            median_slope = find_median_slope(slopes);
        else
        {
            /// if no slope exists then call the function again with possible candidates
            find_edge(candidates, median);
        }
        ///seperate the pairs in small large and equal , by compairing their slope with the median slope
        vector<pair<Vector2, Vector2>> small;
        vector<pair<Vector2, Vector2>> equal;
        vector<pair<Vector2, Vector2>> large;
        for (int i = 0; i < m; i++)
        {
            if (pairs[i].first.x == pairs[i].second.x)
                continue;
            float temp = ((float)(-pairs[i].second.y + pairs[i].first.y)) / ((float)(pairs[i].second.x - pairs[i].first.x));
            float diff = (temp - median_slope);
            if (diff < 0)
                diff = -diff;
            if (diff == 0)
            {
                equal.push_back(pairs[i]);
            }
            else if (temp > median_slope)
            {
                large.push_back(pairs[i]);
            }
            else
            {
                small.push_back(pairs[i]);
            }
        }
        float min_c = INT_MAX;
        /// find the min c intercept of all points with median slope
        for (int i = 0; i < n; i++)
        {
            float cur_c = -points[i].y - median_slope * points[i].x;
            min_c = min(min_c, cur_c);
        }
        Vector2 pmin;
        Vector2 pmax;
        pmin.x = INT_MAX;
        pmax.x = INT_MIN;
        /// find leftmost and rightmost point with that intercept
        for (int i = 0; i < n; i++)
        {
            float cur_c = -points[i].y - median_slope * points[i].x;
            float diff = (cur_c - min_c);
            if (diff < 0)
                diff = -diff;
            if (diff <= 0.0001)
            {
                if (points[i].x > pmax.x)
                {
                    pmax = points[i];
                }
                if (points[i].x < pmin.x)
                {
                    pmin = points[i];
                }
            }
        }
        ///if they lie on the opposite sides of the median this is the bridge , hence return the edge formed
        if (pmin.x <= median.x && pmax.x > median.x)
        {
            return {pmin, pmax};
        }
        // if (pmin.x < median.x && pmax.x >= median.x)
        // {
        //     return {pmin, pmax};
        // }
        // if(pmin.x == pmax.x && pmin.x==median.x)
        // {
        //     return {pmin,pmin};
        // }
        ///if pmax is lesser than the median
        ///add the point with higher x coordinate to candidates if the slope of the pair is equal or smaller than the median slope
        /// add both points if the slope is larger than the median slope
        else if (pmax.x <= median.x)
        {
            for (int i = 0; i < (int)equal.size(); i++)
            {
                Vector2 pt = equal[i].second;
                candidates.push_back(pt);
            }
            for (int i = 0; i < (int)small.size(); i++)
            {
                Vector2 pt = small[i].second;
                candidates.push_back(pt);
            }
            for (int i = 0; i < (int)large.size(); i++)
            {
                Vector2 pt1 = large[i].first;
                candidates.push_back(pt1);
                Vector2 pt2 = large[i].second;
                candidates.push_back(pt2);
            }
            return find_edge(candidates, median);
        }
        ///if pmin is greater than the median
        ///add the point with higher x coordinate tocandidates if the slope of the pair is equal or larger than the median slope
        /// add both points if the slope is smaller than the median slope
        else //if(pmin.x>median.x)
        {
            for (int i = 0; i < (int)equal.size(); i++)
            {
                // Point pt = EQUAL.get(i).get_second();
                Vector2 pt = equal[i].first;
                candidates.push_back(pt);
            }
            for (int i = 0; i < (int)small.size(); i++)
            {

                Vector2 pt1 = small[i].first;
                candidates.push_back(pt1);
                Vector2 pt2 = small[i].second;
                candidates.push_back(pt2);
            }
            for (int i = 0; i < (int)large.size(); i++)
            {
                Vector2 pt = large[i].first;
                candidates.push_back(pt);
            }
            return find_edge(candidates, median);
        }
        /// recursively call the function with points =  candidates
    }

    /// @brief function to calculate the lower hull
    ///normally it recursively calls itself to visualize this instead of using a recursion stack we used a deque to store the states and then call the function with that state
    /// @param points points which can be part of the hull
    /// @param left left bound of the subproblem
    /// @param right right bound of the subproblem
    void find_hull(vector<Vector2> points, Vector2 left, Vector2 right)
    {
        ///terminating condition: handles cases when we end up with left >right then there is no subproblem to solve we return in this case
        if (left.x >= right.x)
            return;
        Vector2 median;
        if (points.size() == 0 || points.size() == 1)
            return;
        /// if number of points less then 2 , then no edge exists
        if (points.size() == 2)
        {
            if (points[0].x < points[1].x)
            {
                median = points[0];
            }
            else
            {
                median = points[1];
            }
        }

        else
        {
            median = find_median(points);
        }
        int flag=0;
        for(auto itr:points)
        {
            if(itr.x>median.x)
            {
                flag=1;
                break;
            }
        }
        if(flag==0)
        {
            for(auto itr:points)
            {
                if(itr.x<median.x)
                {
                    median=itr;
                    break;
                }
            }
        }
        curr_median = median;
        /// find lower bridge by calling find_edge() function
        pair<Vector2, Vector2> edge = find_edge(points, median);
        if(edge.first.x<=median.x && edge.second.x>median.x)
            lower_edges.push_back(edge);
        lower_edges.push_back(edge);
        /// if points.size() ==2 then this is the last edge and there is no subproblem left
        if (points.size() == 2)
            return;
        /// store state for the next subproblem
        if (edge.second.x != right.x)
        {
            struct info info_temp;
            info_temp.left = edge.second;
            info_temp.right = right;
            info_temp.points = points;
            s.push_back(info_temp);
        }
        if (edge.first.x != left.x)
        {
            struct info info_temp;
            info_temp.left = left;
            info_temp.right = edge.first;
            info_temp.points = points;
            s.push_back(info_temp);
        }
    }
    /// @brief this function calls the lower hull function
    ///
    ///it reduces the number of valid points which can be part of the upper hull
    void find_hull_helper(vector<Vector2> points, Vector2 left, Vector2 right)
    {
        vector<Vector2> valid;
        /// all the points present below the left right line can be part of the hull
        for (auto i : points)
        {
            if (orientation(left, right, i) == 2 || orientation(left, right, i) == 0)
            {
                valid.push_back(i);
            }
        }
        if (valid.size() == 0)
            return;
        //valid=points;
        find_hull(valid, left, right);
    }
    /// @brief draws the edges present in the upper hull
    void draw_lower()
    {
        for (auto i : lower_edges)
        {
            DrawLine(i.first.x, i.first.y, i.second.x, i.second.y, WHITE);
        }
    }
};

int main()
{
    cout << "Starting the game..." << endl;
    InitWindow(width, height, "try1");
    SetTargetFPS(60);
    Color green = {20, 168, 133, 255};
    bool select_stage = 1;
    Points points;
    Upper_hull upper_hull;
    Lower_hull lower_hull;
    bool lower = 0;
    bool over = 0;
    Vector2 xmin_upper, xmax_upper;
    Vector2 xmin_lower, xmax_lower;
    bool first = 1;
    float time=1;
    while (WindowShouldClose() == false)
    {
        BeginDrawing();
        if (select_stage)
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                // Get mouse position
                Vector2 mousePosition = GetMousePosition();

                // Print mouse position to console
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

        if (!over && points.points_location.size() > 2 && IsKeyPressed(KEY_ENTER))
        {
            select_stage = 0;
            xmin_upper = upper_hull.find_xmin(points.points_location);
            xmax_upper = upper_hull.find_xmax(points.points_location);
            struct info info_temp1;
            info_temp1.left = xmin_upper;
            info_temp1.right = xmax_upper;
            info_temp1.points = points.points_location;
            upper_hull.s.push_back(info_temp1);

            xmin_lower = lower_hull.find_xmin(points.points_location);
            xmax_lower = lower_hull.find_xmax(points.points_location);
            struct info info_temp2;
            info_temp2.left = xmin_lower;
            info_temp2.right = xmax_lower;
            info_temp2.points = points.points_location;
            lower_hull.s.push_back(info_temp2);
            time=1;
            // cout<<endl<<"found";
        }
        if (!select_stage && !over)
        {
            ///user can increase speed of execution by right arrow
            if(IsKeyPressed(KEY_RIGHT))
            {
                time=0;
            }
            ///reset speed of execution to .1 by left arrow
            if(IsKeyPressed(KEY_LEFT))
            {
                time=1;
            }
            if(IsKeyPressed(KEY_DELETE))
            {
                over = 0;
                select_stage = 1;
                first = 1;
                upper_hull.upper_edges = {};
                lower_hull.lower_edges = {};
                points.points_location = {};
                upper_hull.s={};
                lower_hull.s={};
                continue;
            }

            if (EventTriggered(time))
            {
                if (upper_hull.s.empty())
                {
                    over = 1;
                    lower = 1;
                }
                else
                {
                    struct info info_temp;
                    info_temp = upper_hull.s.front();
                    upper_hull.s.pop_front();
                    upper_hull.find_hull_helper(info_temp.points, info_temp.left, info_temp.right);
                }
            }
        }

        if (lower)
        {
            if(IsKeyPressed(KEY_RIGHT))
            {
                time=0;
            }
            if(IsKeyPressed(KEY_LEFT))
            {
                time=1;
            }
            if(IsKeyPressed(KEY_DELETE))
            {
                over = 0;
                select_stage = 1;
                first = 1;
                upper_hull.upper_edges = {};
                lower_hull.lower_edges = {};
                points.points_location = {};
                upper_hull.s={};
                lower_hull.s={};
                continue;
            }
            if (EventTriggered(time))
            {

                if (lower_hull.s.empty())
                {
                    lower = 0;
                }
                else
                {
                    struct info info_temp;
                    info_temp = lower_hull.s.front();
                    lower_hull.s.pop_front();
                    lower_hull.find_hull_helper(info_temp.points, info_temp.left, info_temp.right);
                    first = 0;
                }
            }
        }

        ClearBackground(green);

        DrawRectangleLines(0, 0, width, height - 50, BLACK);
        if (select_stage == 1)
        {
            if (points.points_location.size() > 2)
            {
                DrawText("hit enter to start ", 20, height - 45, 40, WHITE);
            }
            else
            {
                DrawText("minimum 3 points to start ", 20, height - 45, 40, WHITE);
            }
        }

        else if (over == 1 && !lower)
        {
            DrawText("hit enter to restart ", 20, height - 45, 40, WHITE);
            ///after calculating the upper and lower hull join xmin of both the hulls and then join xmax of both the hulls
            DrawLine(upper_hull.xmin.x, upper_hull.xmin.y, lower_hull.xmin.x, lower_hull.xmin.y, WHITE);

            DrawLine(upper_hull.xmax.x, upper_hull.xmax.y, lower_hull.xmax.x, lower_hull.xmax.y, WHITE);

            if (IsKeyPressed(KEY_ENTER))
            {
                over = 0;
                select_stage = 1;
                first = 1;
                upper_hull.upper_edges = {};
                lower_hull.lower_edges = {};
                points.points_location = {};
            }

            if(IsKeyPressed(KEY_R))
            {
                over=0;
                first = 1;
                upper_hull.upper_edges = {};
                lower_hull.lower_edges = {};

                select_stage = 0;
                xmin_upper = upper_hull.find_xmin(points.points_location);
                xmax_upper = upper_hull.find_xmax(points.points_location);
                struct info info_temp1;
                info_temp1.left = xmin_upper;
                info_temp1.right = xmax_upper;
                info_temp1.points = points.points_location;
                upper_hull.s.push_back(info_temp1);

                xmin_lower = lower_hull.find_xmin(points.points_location);
                xmax_lower = lower_hull.find_xmax(points.points_location);
                struct info info_temp2;
                info_temp2.left = xmin_lower;
                info_temp2.right = xmax_lower;
                info_temp2.points = points.points_location;
                lower_hull.s.push_back(info_temp2);
                time=1;

            }
        }
        else if (lower == 0)
        {
            DrawText("upper hull", 20, height - 45, 40, WHITE);
            DrawLine(upper_hull.curr_median.x, 0, upper_hull.curr_median.x, 500, BLACK);
        }
        else
        {
            DrawText("lower hull", 20, height - 45, 40, WHITE);
            if (!first)
                DrawLine(lower_hull.curr_median.x, 0, lower_hull.curr_median.x, 500, BLACK);
        }
        points.draw();
        upper_hull.draw_upper();
        lower_hull.draw_lower();

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
