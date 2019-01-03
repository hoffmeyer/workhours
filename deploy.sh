#!/bin/sh

error_exit ()
{
  echo "$1" 1>&2
  exit 1
}

cd /var/www/hoffmeyers.net/public_html/client || error_exit "error changing directory!. now exiting..."
yarn install || error_exit "error running yarn install in client! now exiting ..."
yarn run build || error_exit "error running yarn build in client! now exiting ..."
yarn run webpack:production || error_exit "error running yarn webpack in client! now exiting ..."
cd ..
yarn install || error_exit "error running npm install! now exiting ..."
yarn run build || error_exit "error building project! now exiting ..."
