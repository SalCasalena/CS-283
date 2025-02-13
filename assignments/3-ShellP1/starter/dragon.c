#include <stdio.h>
#include "dragon.h"

// The ASCII representation of the Drexel Dragon
const char *dragon_ascii[] = {
    "                                                                        @%%%%                       ",
    "                                                                     %%%%%%                         ",
    "                                                                    %%%%%%                          ",
    "                                                                 % %%%%%%%           @              ",
    "                                                                %%%%%%%%%%        %%%%%%%           ",
    "                                       %%%%%%%  %%%%@         %%%%%%%%%%%%@    %%%%%%  @%%%%        ",
    "                                  %%%%%%%%%%%%%%%%%%%%%%      %%%%%%%%%%%%%%%%%%%%%%%%%%%%          ",
    "                                %%%%%%%%%%%%%%%%%%%%%%%%%%   %%%%%%%%%%%% %%%%%%%%%%%%%%%           ",
    "                               %%%%%%%%%%%%%%%%%%%%%%%%%%%%% %%%%%%%%%%%%%%%%%%%     %%%            ",
    "                             %%%%%%%%%%%%%%%%%%%%%%%%%%%%@ @%%%%%%%%%%%%%%%%%%        %%            ",
    "                            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% %%%%%%%%%%%%%%%%%%%%%%                ",
    "                            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%              ",
    "                            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%%%%%%@              ",
    "      %%%%%%%%@           %%%%%%%%%%%%%%%%        %%%%%%%%%%%%%%%%%%%%%%%%%%      %%                ",
    "    %%%%%%%%%%%%%         %%@%%%%%%%%%%%%           %%%%%%%%%%% %%%%%%%%%%%%      @%                ",
    "  %%%%%%%%%%   %%%        %%%%%%%%%%%%%%            %%%%%%%%%%%%%%%%%%%%%%%%                        ",
    " %%%%%%%%%       %         %%%%%%%%%%%%%             %%%%%%%%%%%%@%%%%%%%%%%%                       ",
    "%%%%%%%%%@                % %%%%%%%%%%%%%            @%%%%%%%%%%%%%%%%%%%%%%%%%                     ",
    "%%%%%%%%@                 %%@%%%%%%%%%%%%            @%%%%%%%%%%%%%%%%%%%%%%%%%%%%                  ",
    "%%%%%%%@                   %%%%%%%%%%%%%%%           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%              ",
    "%%%%%%%%%%                  %%%%%%%%%%%%%%%          %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%      %%%%  ",
    "%%%%%%%%%@                   @%%%%%%%%%%%%%%         %%%%%%%%%%%%@ %%%% %%%%%%%%%%%%%%%%%   %%%%%%%%",
    "%%%%%%%%%%                  %%%%%%%%%%%%%%%%%        %%%%%%%%%%%%%      %%%%%%%%%%%%%%%%%% %%%%%%%%%",
    "%%%%%%%%%@%%@                %%%%%%%%%%%%%%%%@       %%%%%%%%%%%%%%     %%%%%%%%%%%%%%%%%%%%%%%%  %%",
    " %%%%%%%%%%                  % %%%%%%%%%%%%%%@        %%%%%%%%%%%%%%   %%%%%%%%%%%%%%%%%%%%%%%%%% %%",
    "  %%%%%%%%%%%%  @           %%%%%%%%%%%%%%%%%%        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  %%% ",
    "   %%%%%%%%%%%%% %%  %  %@ %%%%%%%%%%%%%%%%%%          %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    %%% ",
    "    %%%%%%%%%%%%%%%%%% %%%%%%%%%%%%%%%%%%%%%%           @%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    %%%%%%% ",
    "     %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%              %%%%%%%%%%%%%%%%%%%%%%%%%%%%        %%%   ",
    "      @%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%                  %%%%%%%%%%%%%%%%%%%%%%%%%               ",
    "        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%                      %%%%%%%%%%%%%%%%%%%  %%%%%%%          ",
    "           %%%%%%%%%%%%%%%%%%%%%%%%%%                           %%%%%%%%%%%%%%%  @%%%%%%%%%         ",
    "              %%%%%%%%%%%%%%%%%%%%           @%@%                  @%%%%%%%%%%%%%%%%%%   %%%        ",
    "                  %%%%%%%%%%%%%%%        %%%%%%%%%%                    %%%%%%%%%%%%%%%    %         ",
    "                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%                      %%%%%%%%%%%%%%            ",
    "                %%%%%%%%%%%%%%%%%%%%%%%%%%  %%%% %%%                      %%%%%%%%%%  %%%@          ",
    "                     %%%%%%%%%%%%%%%%%%% %%%%%% %%                          %%%%%%%%%%%%%@          ",
    "                                                                                 %%%%%%%@        ",
    NULL // Marks the end of the ASCII array
};

// Function to print the ASCII dragon
void print_dragon() {
    for (int i = 0; dragon_ascii[i] != NULL; i++) {
        printf("%s\n", dragon_ascii[i]);
    }
}
