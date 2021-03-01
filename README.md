# repo-iter3-20-12
Repo for repo-iter3-20-12

jann0050

gongx128

omarx030

lu000097

## Pre-requisites
  * [Git](https://git-scm.com/)

## Docker Pre-requisites
  * Windows 10 Home
    * Install [wsl2 and Ubuntu](https://www.youtube.com/watch?v=ilKQHAFeQR0&list=RDCMUCzLbHrU7U3cUDNQWWAqjceA&start_radio=1&t=7)
  * Install [Docker Desktop](https://hub.docker.com/?overlay=onboarding) from [Docker Hub](https://hub.docker.com/)
  * Linux
    * Use [docker group instead of sudo](https://www.digitalocean.com/community/tutorials/how-to-install-and-use-docker-on-ubuntu-18-04)

## Getting Started with Docker

1. Build docker image

    ```bash
    bin/build-env.sh
    ```

2. Run docker image

    ```bash
    #Usage bin/run-env.sh <port - optional(default 8081)>
    bin/run-env.sh
    ```
    
3. Build project web server (inside docker image) NOTE: You won't be able to `cd` here yet because the project directory does not exist. If you were able to launch the above commands you should now be inside the docker image. You can exit it with CTRL+D now.

    ```bash
    # Inside docker image
    cd /home/user/repo/project
    make
    ```
    
4. Run web server (inside docker image)

    ```bash
    # MUST be within project/ directory inside docker image
    cd /home/user/repo/project
    ./bin/run.sh
    ```
    
5. Open up Firefox and browse to http://127.0.0.1:8081/


## Getting Started using SSH on CSE Lab machines

1. Initial Setup of remote web server (full copy)

    ```bash
    #Example: ./project/bin/setup-server.sh dtorban 05
    ./project/bin/setup-server.sh <username> <computer_number>
    ```

2. Run remote web server (only copies src directory)

    ```bash
    #Example: ./project/bin/server.sh dtorban 05 8081
    ./project/bin/server.sh <username> <computer_number> <port>
    ```

    
5. Open up Firefox and browse to http://127.0.0.1:8081/

## Getting Started with VOLE-3D or VOLE

1. Log on to [VOLE-3D or VOLE](https://vole.cse.umn.edu/) (VOLE is slower for graphics, but is great for tests, etc...)

2. Build project

    ```bash
    cd /path/to/cloned/repository
    cd project
    make
    ```
    
 2. Run project

    ```bash
    # MUST be within project/ directory
    ./bin/run.sh
    ```
    
 5. Open up Firefox and browse to http://127.0.0.1:8081/
