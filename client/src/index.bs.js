// Generated by BUCKLESCRIPT VERSION 4.0.5, PLEASE EDIT WITH CARE
'use strict';

var ReactDOMRe = require("reason-react/src/ReactDOMRe.js");
var ReasonReact = require("reason-react/src/ReasonReact.js");
var App$Workhours = require("./app.bs.js");
var Router$Workhours = require("./Router.bs.js");
var RegisterServiceWorker = require("./registerServiceWorker");

((require('./index.css')));

ReactDOMRe.renderToElementWithId(ReasonReact.element(undefined, undefined, Router$Workhours.WithRouter[/* make */1]((function (currentRoute) {
                return ReasonReact.element(undefined, undefined, App$Workhours.make(currentRoute, /* array */[]));
              }))), "root");

RegisterServiceWorker.default();

/*  Not a pure module */
