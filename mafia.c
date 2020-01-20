//Team: Grace Mao Amanda Zheng George Zhou (Team Gag)
#include "networking.c"
//Amanda's Code
char **players;
int *roles; //0 is regular person, 1 is mafia, 2 is detective, 3 is nurse
int *votes;
int maf, nur, det, mdone, ddone, ndone, num_players;
char *username;
char *victim;
int game_over = 0;
int game_start = 0;
int num_players, num_day, type_night, num_night; //type night is for the roles, while num night is how many nights have passed
int night;

struct turns {
    char **member;
    int index;
};
struct turns m_turn;
struct turns n_turn;
struct turns d_turn;
//method for length of char **
int len_double(char **ary) {
    int count = 0;
    for (count = 0; ary[count] != NULL; count++) {
        //omg isn't this so cool!!! this actually works
    }
    return count;
}
//havent figured out exact numbers yet
//number of people based off of users (number of players)
int mafiaNum(int users) {
    int mafia = 0;
    if (users > 8) {
        mafia = 3;
    } else if (users > 6) {
        mafia = 2;
    } else {
        mafia = 1;
    }
    maf = mafia;
    printf("Total Mafia Members: %d\n", mafia);
    return mafia;
}

//number of nurse based off of users (number of players)
int nurseNum(int users) {
    int nurse = 0;
    if (users > 7) {
        nurse = 2;
    } else if (users > 2) {
        nurse = 1;
    } else {
      nurse = 0;
    }
    nur = nurse;
    printf("Total Number of Nurse(s): %d\n", nurse);
    return nurse;
}

//number of dectecive based off of users (number of players)
int detectiveNum(int users) {
    int detective;
    if (users > 10) {
        detective = 2;
    } else {
        detective = 1;
    }
    det = detective;
    printf("Total Detective(s): %d\n", detective);
    return detective;
}

void printint(int *list) {
    printf("starting to print\n\n");
    for(size_t i = 0; i < num_players; i++){
        printf("%d,", list[i]);
    }
    printf("\n");
}
//need to initialize turns to 0
void genRoles() {
    int *assign = calloc(num_players, sizeof(int));
    int total = num_players;
    m_turn.member = calloc(maf, sizeof(char*));
    d_turn.member = calloc(det, sizeof(char*));
    n_turn.member = calloc(nur, sizeof(char*));
    m_turn.index = 0;
    d_turn.index = 0;
    n_turn.index = 0;
    for (size_t i = 0; i < maf; i++) {
        m_turn.member[m_turn.index] = malloc(sizeof(char) * 1000);
        m_turn.index++;
        strcpy(m_turn.member[i], "\0");
        assign[i] = 1;
    }
    for (size_t i = 0; i < det; i++) {
        d_turn.member[d_turn.index] = malloc(sizeof(char) * 1000);
        d_turn.index++;
        strcpy(d_turn.member[i], "\0");
        assign[maf + i] = 2;
    }
    for (size_t i = 0; i < nur; i++) {
        n_turn.member[n_turn.index] = malloc(sizeof(char) * 1000);
        n_turn.index++;
        strcpy(n_turn.member[i], "\0");
        assign[maf + det + i] = 3;
    }
    for (size_t i = maf + nur + det; i < num_players; i++) {
        assign[i]=0;
    }
    m_turn.index = 0;
    d_turn.index = 0;
    n_turn.index = 0;
    unsigned int r;
    printf("woah\n");
    printint(assign);
    for (size_t i = 0; i < num_players; i++) {
        int index = rand() % total;
        r = assign[index];
        printf("r = %d\n", r);
        if (r == 1) {
            roles[i] = 1;
            m_turn.member[m_turn.index] = players[i];
            m_turn.index++;
        } else if (r == 2) {
            roles[i] = 2;
            d_turn.member[d_turn.index] = players[i];
            d_turn.index++;
        } else if(r == 3) {
            roles[i] = 3;
            n_turn.member[n_turn.index] = players[i];
            n_turn.index++;
        } else {
          roles[i] = 0;
        }
        total--;
        int temp = assign[total];
        assign[total] = assign[r];
        assign[r] = assign[total];
    }
    printint(roles);
}

int getRole(char *check) {
    for (size_t i = 0; i < num_players; i++) {
        if (strcmp(players[i], check) == 0) {
            return i;
        }
    }
    return -1;
}

int len_single(char *ary) {
    int count = 0;
    for (count = 0; &ary[count] != NULL; count++) {
        //omg isn't this so cool!!! this actually works
    }
    return count;
}

char *to_string(char **ary) {
    int size = 0;
    int i = 0;
    for (; i < len_double(ary); i++) {
        size += len_single(ary[i]);
        size += 2;
    }
    size -= 1;
    char *line = malloc(size *sizeof(char));
    line[0] = 0;
    for (i = 0; i < len_double(ary); i++) {
        if (strcmp(ary[i], " ") != 0) { //" " means the player has died
            strcat(line, ary[i]);
        }
        if (i < len_double(ary) - 1) {
            strcat(line, ", ");
        }
    }
    return line;
}

void print_players() {
    printf("In Game: ");
    for (size_t i = 0; i < num_players; i++) {
        printf("%s",players[i]);
        if (i != num_players - 1) {
            printf(",");
        } else {
            printf("\n");
        }
    }
}

int remove_name(char **ary, char *name) {
    int i = 0;
    for (; i < len_double(ary); i++) {
        if (strcmp(ary[i], name) == 0) {
            strcpy(ary[i], " ");
            return 1;
        }
    }
    return 0;
}

char **parse_args(char *line, char *del) {
    char *token;
    char **tokens;
    tokens = malloc(sizeof(char*) * 10);
    int i = 0;
    while (line != NULL) {
        token = strsep(&line, del);
        tokens[i] = token;
        i++;
    }
    tokens[i] = 0;
    return tokens;
}

int valid(char *name) {
    int i = 0;
    for (; i < len_double(players); i++) {
        if (strcmp(players[i], name) == 0) {
            return 1;
        }
    }
    return 0;
}

// notes:
// num_players attainable through length of users later
// OR maybe game start when num_players equals length of char ** with usernames
// going with assumption that at least 5 people needed, will change later

// LATER SHOULD BE MOVED TO MAFIA.C

int main() {
    int sd_conn, game_start = -1;
    char buffer[BUFFER_SIZE] = "not yet";
    num_players = 0;
    num_night = 0;
    num_day = 0;
    username = malloc(sizeof(char) * 1000);
    victim = malloc(sizeof(char) * 1000);
    strcpy(username, "\0");
    sd_conn = client_setup(TEST_IP);
    players = calloc(12, sizeof(char*));
    roles = calloc(12, sizeof(int));
    for (size_t i = 0; i < 12; i++) {
        players[i] = malloc(sizeof(char) * 1000);
        strcpy(players[i], "\0");
        roles[i] = -1;
    }
    if (sd_conn >= 0) {
        printf("Waiting for players to join...\n");
        while (game_start == -1 && read(sd_conn, buffer, sizeof(buffer))) {
            // client reads from subserver
            if (strcmp(buffer, "Start\n") == 0) {
                // client reads Start from subserver
                strcpy(buffer, "Game Started");
            } else if ((strlen(buffer) == 4) && (buffer[0] == 'N')) {
                // client reads number of players from subserver
                num_players = buffer[3] - 96;
                game_start = 0;
                printf("\n\n\nLET'S BEGIN!\n\n\n");
            }
        }
        srand(time(NULL));
        if (strcmp(username, "\0") == 0) {
            printf("\\Mafia$ Enter Username: ");
            fgets(buffer, 1000, stdin); // reads username from stdin
            buffer[strlen(buffer) - 1] = '\0';
            printf("Your Username is: %s\n", buffer);
            strcpy(username, buffer); // username copied
            printf("\\Mafia$ Waiting for other players...\n");
            strcpy(buffer, "\0");
            strcpy(buffer, "U");
            strncat(buffer, username, strlen(username));
            write(sd_conn, buffer, sizeof(buffer));
            // client writes username to subserver
        }
        while (game_start == 0 && read(sd_conn, buffer, sizeof(buffer))) {
            // client reads list of usernames from subserver
            if (buffer[0] == 'U') {
                memmove(buffer, buffer + 1,strlen(buffer));
                players = parse_args(buffer, ",");
                game_start = 1;
            }
        }
        print_players();
        printf("\\Mafia$ Generating Role...\n\n");
        mafiaNum(num_players);
        detectiveNum(num_players);
        nurseNum(num_players);
        printf("\n");
        genRoles();
        printf("what\n");
        printint(roles);
        printf("getrole %d\n",getRole(username));
        if (roles[getRole(username)] == 0) {
            printf("Your Role: Civilian\n");
        } else if (roles[getRole(username)] == 1) {
            printf("Your Role: Mafia\n");
        } else if (roles[getRole(username) == 2]) {
            printf("Your Role: Detective\n");
        } else {
            printf("Your Role: Nurse\n");
        }
        printf("\n");
        game_start = 1;
        night = 1;
        num_day = 1;
        int type_day = 0;
        num_night = 1;
        type_night = 0;
        votes = calloc(num_players, sizeof(int));
        for (int i = 0; i < num_players; i++) {
            votes[i] = 0;
        }
        m_turn.index = 0;
        d_turn.index = 0;
        n_turn.index = 0;
        mdone = 0;
        ndone = 0;
        ddone = 0;
        printf("all mafs\n");
        for (int i = 0; i < maf; i++){
            printf("%s\n", m_turn.member[i]);
        }
        while (!game_over) {
            if (!night) { //daytime
                printf("It's Daytime!\n");
                if (num_day == 1) {
                    printf("Welcome to Mafia!\n");
                    sleep(2);
                    printf("The night will begin shortly...\n");
                } else {
                    if (type_day == 0) {
                        if (strcmp(victim, "") == 0) {
                            printf("Nobody Died!\n");
                        } else {
                            printf("%s has died!\n", victim);
                            if (strcmp(username, victim) == 0) {
                                game_over = 1;
                            } else {
                                printf("You have 5 minutes to discuss.\n"); //George's Timer
                            }
                        }
                    } else if (type_day == 1) {
                        //chatbox
                    } else {
                        //voting
                    }
                }
            } else { //nighttime
                if (type_night == 0) {
                    printf("Waiting for Mafia\n");
                    printf("voting right now %s\n", m_turn.member[m_turn.index]);
                    printf("askdjfa\n");
                    if (strcmp(username, m_turn.member[m_turn.index]) == 0) {
                      //  printf("ajkdf\n");
                        printf("Here are all of your victims: \n");
                        print_players();
                        printf("\\Vote for your victim: ");
                        fgets(victim, 1000, stdin);
                        victim[strlen(victim)] = '\0';
                        /*while (!valid(&buffer)) { //function to see if its valid victim
                            printf("\nYou have voted for an invalid victim.%s\n Here are all of your victims\n", to_string(players));
                            printf("\\Vote for your victim: ");
                            fgets(buffer, 1000, stdin);
                            buffer[strlen(buffer) - 1] = '\0';
                        }*/
                        printf("\nYou have selected to kill: %s\n", victim);
                        write(sd_conn, victim, sizeof(victim));
                        m_turn.index++;
                    } else {
                        strcpy(buffer, "done");
                        write(sd_conn, buffer, sizeof(buffer));
                    }
                    read(sd_conn, buffer, sizeof(buffer));// block until server sends signal
                    type_night++;
                    printf("\n\n\nNext\n\n\n");
                }
                if (type_night == 1) {
                    printf("Waiting for Detective\n");
                    if (strcmp(username, d_turn.member[d_turn.index]) == 0) {
                        printf("Here are all of your suspects: %s\n", to_string(players));
                        printf("\\Choose to investigate a suspects: ");
                        fgets(buffer, 1000, stdin);
                        buffer[strlen(buffer) - 1] = '\0';
                        /*while (!valid(&buffer)) { //function to see if its valid victim
                            printf("\nYou have chosen an invalid suspect.\n Here are all of your suspects: %s\n", to_string(players));
                            printf("\\Choose to investigate a suspects: ");
                            fgets(buffer, 1000, stdin);
                            buffer[strlen(buffer) - 1] = '\0';
                        }*/
                        printf("\nYou have chosen to investigate: %s\n", buffer);
                        sleep(2);
                        if (getRole(buffer) == 0) {
                            printf("%s's identity is: Civilian\n", buffer);
                        } else if (getRole(buffer) == 1) {
                            printf("%s's identity is: Mafia\n", buffer);
                        } else if (getRole(buffer) == 2) {
                            printf("%s's identity is: Detective\n", buffer);
                        } else {
                            printf("%s's identity is: Nurse\n", buffer);
                        }
                    }
                    d_turn.index++;
                    type_night++;
                }
                if (type_night = 2) {
                    printf("Waiting for Nurse\n");
                    if (strcmp(username, n_turn.member[n_turn.index]) == 0) {
                        printf("Here are all of your patients: %s\n", to_string(players));
                        printf("\\Choose to save a patients: ");
                        fgets(buffer, 1000, stdin);
                        buffer[strlen(buffer) - 1] = '\0';
                        /*while (!valid(&buffer)) { //function to see if its valid victim
                            printf("\nYou have voted for an invalid victim.\n Here are all of your patients: %s\n", to_string(players));
                            printf("\\Choose to save a patient: ");
                            fgets(buffer, 1000, stdin);
                            buffer[strlen(buffer)-1] = '\0';
                        }*/
                        printf("\nYou have chosent to save: %s\n", buffer);
                        write(sd_conn, buffer, sizeof(buffer));
                        n_turn.index++;
                    } else {
                        strcpy(buffer, "done");
                        write(sd_conn, buffer, sizeof(buffer));
                    }
                }
                num_night++;
                read(sd_conn, buffer, sizeof(buffer));
                if (strcmp(buffer, username) == 0) { //checking to see if he dead
                    printf("Unfortunately, you have DIED\n");
                    strcmp(buffer, "died");
                    write(sd_conn, buffer, sizeof(buffer));
                    sleep(10);
                    return 0;
                } else {
                    printf("Congradulations, you have SURVIVED the night\n");
                    strcmp(buffer, "surv");
                    write(sd_conn, buffer, sizeof(buffer));
                }
            }
        }
    }
    return 0;
}
