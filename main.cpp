#include <simplecpp>
#include <sstream>
#include <cstdlib>
#include "shooter.h"
#include "bubble.h"

/* Simulation Vars */
const double STEP_TIME = 0.02;

/* Game Vars */
//const int PLAY_Y_HEIGHT = 450;
const int LEFT_MARGIN = 70;
const int TOP_MARGIN = 20;
const int BOTTOM_MARGIN = (PLAY_Y_HEIGHT+TOP_MARGIN);

string int_to_string(int &change_int)
{
    stringstream ss;
    ss<<change_int;
    string change_str;
    ss>>change_str;
    return change_str;
}

// Checks for collision b/w bullet and bubble, taking both as reference to avoid delay due to copying of objects
bool intersects_bb(Bubble &bub, Bullet &bul)
{
    int dist_x = abs(bub.get_center_x() - bul.get_center_x());
    int dist_y = abs(bub.get_center_y() - bul.get_center_y());

    if ((dist_x > (bul.get_width()/2.0 + bub.get_radius())) || (dist_y > (bul.get_height()/2.0 + bub.get_radius())))
        return false;

    if ((dist_x <= (bul.get_width()/2.0)) || (dist_y <= (bul.get_height()/2.0)))
        return true;

    int corner_distance_sq = (dist_x - bul.get_width()/2.0)*(dist_x - bul.get_width()/2.0) + (dist_y - bul.get_height()/2.0)*(dist_y - bul.get_height()/2.0);

    return (corner_distance_sq <= (bub.get_radius()*bub.get_radius()));
}

// Checks for collision b/w bubble and body of the shooter
bool intersects_bs_body(Bubble &bub, Shooter &shoot){
    int dist_x = abs(bub.get_center_x() - shoot.get_body_center_x());
    int dist_y = abs(bub.get_center_y() - shoot.get_body_center_y());

    if ((dist_x > (shoot.get_body_width()/2.0 + bub.get_radius())) || (dist_y > (shoot.get_body_height()/2.0 + bub.get_radius())))
        return false;

    if ((dist_x <= (shoot.get_body_width()/2.0)) || (dist_y <= (shoot.get_body_height()/2.0)))
        return true;

    int corner_distance_sq = (dist_x - shoot.get_body_width()/2.0)*(dist_x - shoot.get_body_width()/2.0) + (dist_y - shoot.get_body_height()/2.0)*(dist_y - shoot.get_body_height()/2.0);

    return (corner_distance_sq <= (bub.get_radius()*bub.get_radius()));
}

// Checks for collision b/w bubble and head of the shooter
bool intersects_bs_head(Bubble &bub, Shooter &shoot){
    int dx = bub.get_center_x() - shoot.get_head_center_x();
    int dy = bub.get_center_y() - shoot.get_head_center_y();
    if (dx*dx + dy*dy < (bub.get_radius() + shoot.get_head_radius())*(bub.get_radius() + shoot.get_head_radius()))
        return true;
    return false;
}

// moves the bullets
void move_bullets(vector<Bullet> &bullets){
    // move all bullets
    for(unsigned int i=0; i < bullets.size(); i++){
        if(!bullets[i].nextStep(STEP_TIME)){
            bullets.erase(bullets.begin()+i);
            i--; //Making sure the bullet after the erased bullet doesn't get skipped
        }
    }
}
// moves the bubbles and checks whether
int move_bubbles(vector<Bubble> &bubbles, vector<Bullet> &bullets, set<int> &bub_labels, Shooter &shooter, int &game_score, int &health, int &label_bubble){
    // move all bubbles
    for (unsigned int i=0; i < bubbles.size(); i++)
    {
        bubbles[i].nextStep(STEP_TIME);
        bool is_blue = false;
        // checking if bubble intersects the shooter, if it does, insert the label of the bubble into the set
        if ((intersects_bs_head(bubbles[i],shooter)) || (intersects_bs_body(bubbles[i],shooter))){
            if (bub_labels.find(bubbles[i].get_bubble_number()) == bub_labels.end()){
                bub_labels.insert(bubbles[i].get_bubble_number());
                health--;
                shooter.change_color_to_blue();
                is_blue = true;
            }
        }
        //removing the bubble label if it doesn't intersect the shooter
        else {
            bub_labels.erase(bubbles[i].get_bubble_number());
        }

        //returning 0 if health is 0, will stop the game later
        if (health == 0) return 0;
        for (unsigned int j=0; j < bullets.size(); j++){

            // Checking if bubble and bullet intersect
            if (intersects_bb(bubbles[i],bullets[j])){
                // If the bubble is a large one, divide it into two smaller bubbles
                if (bubbles[i].get_radius() != BUBBLE_DEFAULT_RADIUS)
                {
                    bubbles.push_back(Bubble(bubbles[i].get_center_x(), bubbles[i].get_center_y(), bubbles[i].get_radius()/2, -1*bubbles[i].get_vx(), 0, bubbles[i].get_color(), label_bubble));
                    label_bubble++;
                    bubbles.push_back(Bubble(bubbles[i].get_center_x(), bubbles[i].get_center_y(), bubbles[i].get_radius()/2, bubbles[i].get_vx(), 0, bubbles[i].get_color(), label_bubble));
                    label_bubble++;
                }
                //Erase the bigger bubble and the bullet
                bullets.erase(bullets.begin()+j);
                bubbles.erase(bubbles.begin()+i);
                //Increment the score and decrement both i and j so that you don't forget to check one bullet and bubble
                game_score+=1;
                i--;
                j--;
                break;
            }
        }

        if (is_blue) {
            shooter.change_color_to_green();
        }

    }
    //return 1 when health is not 0
    return 1;
}

vector<Bubble> create_bubbles(int &level, int &label_bubble)
{
    // create initial bubbles in the game
    vector<Bubble> bubbles;
    //Generating pseudo-random colours for each level
    unsigned int color_red = rand()%256;
    unsigned int color_green = rand()%256;
    unsigned int color_blue = rand()%256;

    //Adding the initial bubbles, level+1 bubbles for each level (Otherwise the game becomes too hard)
    for (int i = 1; i <= level + 1; i++)
    {
        bubbles.push_back(Bubble((double)WINDOW_X*i/(level+2), BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS*pow(2,level-1), pow(-1,i)*BUBBLE_DEFAULT_VX*i, 0, simplecpp::COLOR(color_red, color_green, color_blue), label_bubble));
        label_bubble++;
    }
    return bubbles;
}


int main()
{
    initCanvas("Bubble Trouble", WINDOW_X, WINDOW_Y);

    Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
    b1.setColor(COLOR(0, 0, 255));

    string msg_cmd("Cmd: _");
    Text charPressed(LEFT_MARGIN, BOTTOM_MARGIN, msg_cmd);

    // Intialize the shooter
    Shooter shooter(SHOOTER_START_X, SHOOTER_START_Y, SHOOTER_VX);

    // Initialize the bullets (empty)
    vector<Bullet> bullets;

    XEvent event;

    // Initialize score
    int game_score = 0;
    string str_score = int_to_string(game_score);
    string msg_score = "Score:" + str_score;
    Text text_score(WINDOW_X - LEFT_MARGIN, BOTTOM_MARGIN, msg_score);

    // Initialize bool to determine whether the game should stop or not
    bool stop_game = false;

    // Main game loop
    for (int level = 1; level <= 3; level++)
    {
        // Keeps track of what label we have to assign the next newly created bubble
        int label_bubble = 1;

        // Initialize the bubbles
        vector<Bubble> bubbles = create_bubbles(level, label_bubble);

        // Initialize set which keeps track of the bubbles(by their integer labels defined in the Bubble class) that have collided with the shooter
        set<int> bub_labels;

        // Initialize time and the display text on the screen
        double game_time = 0.0;
        int int_game_time = (int)game_time;
        int total_game_time = 30 - 5*level;

        string str_time = int_to_string(int_game_time);
        string str_total_time = int_to_string(total_game_time);

        string msg_time("Time :" + str_time + "/" + str_total_time);
        Text text_time(LEFT_MARGIN, TOP_MARGIN, msg_time);

        // Initialize health and the text
        int health = 3;
        string str_health = int_to_string(health);
        string msg_health("Health :" + str_health + "/3");
        Text text_health(WINDOW_X - LEFT_MARGIN, TOP_MARGIN, msg_health);

        // Text for current level
        string str_level = int_to_string(level);
        string msg_level("Level :" + str_level + "/3");
        Text text_level(WINDOW_X/2.0, BOTTOM_MARGIN, msg_level);

        string msg_level1("Level " + str_level);
        Text text_level1(WINDOW_X/2.0, WINDOW_Y/2.0, msg_level1);
        wait(2);
        text_level1.hide();

        while (true)
        {

            bool pendingEvent = checkEvent(event);
            if (pendingEvent)
            {

                // Get the key pressed
                char c = charFromEvent(event);
                msg_cmd[msg_cmd.length() - 1] = c;
                charPressed.setMessage(msg_cmd);

                // Update the shooter
                if(c == 'a')
                    shooter.move(STEP_TIME, true);
                else if(c == 'd')
                    shooter.move(STEP_TIME, false);
                else if(c == 'w')
                    bullets.push_back(shooter.shoot());
                else if(c == 'q')
                    return 0;
            }

            // Update the bullets
            move_bullets(bullets);

            // Update the bubbles (when health becomes 0, cont_game becomes 0)
            int cont_game = move_bubbles(bubbles, bullets, bub_labels, shooter, game_score, health, label_bubble);

            // Display score
            string str_score_new = int_to_string(game_score);
            msg_score = "Score:" + str_score_new;
            text_score.setMessage(msg_score);

            // Display health
            string str_health_new = int_to_string(health);
            msg_health = "Health :" + str_health_new + "/3";
            text_health.setMessage(msg_health);

            // Ending the current level - Moving to the next level or winning
            if (bubbles.empty()){
                bullets = {};
                if (level!=3) {
                    Text t1(WINDOW_X/2,WINDOW_Y/2,"Congrats! On to the next level!");
                    t1.setColor(COLOR("green"));
                    wait(3);
                }

                else {
                    Text t1(WINDOW_X/2,WINDOW_Y/2,"Congrats! You Won!");
                    t1.setColor(COLOR("green"));
                    wait(3);
                }
                break;
            }

            //Exiting the game when health = 0
            if (cont_game == 0){
                Text t1(WINDOW_X/2,WINDOW_Y/2,"Game Over");
                t1.setColor(COLOR("red"));
                stop_game = true;
                wait(5);
                break;
            }

            //Updating game time (not real world time though)
            wait(STEP_TIME);
            game_time+=STEP_TIME;

            //Display time
            int_game_time = (int)game_time;
            string str_time_new = int_to_string(int_game_time);
            msg_time = "Time:" + str_time_new + "/" + str_total_time;
            text_time.setMessage(msg_time);

            //Ending the game if the time is over
            if ((int)game_time == total_game_time){
                Text t1(WINDOW_X/2,WINDOW_Y/2,"Time Over");
                t1.scale(5);
                t1.setColor(COLOR("red"));
                stop_game = true;
                wait(5);
                break;
            }
        }
    if (stop_game) break;
    }
}
