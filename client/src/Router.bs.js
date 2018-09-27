// Generated by BUCKLESCRIPT VERSION 4.0.5, PLEASE EDIT WITH CARE
'use strict';

var Cn = require("re-classnames/src/Cn.bs.js");
var Block = require("bs-platform/lib/js/block.js");
var Curry = require("bs-platform/lib/js/curry.js");
var ReasonReact = require("reason-react/src/ReasonReact.js");
var Config$Workhours = require("./Config.bs.js");

var component = ReasonReact.reducerComponent("WithRouter");

function make(children) {
  return /* record */[
          /* debugName */component[/* debugName */0],
          /* reactClassInternal */component[/* reactClassInternal */1],
          /* handedOffState */component[/* handedOffState */2],
          /* willReceiveProps */component[/* willReceiveProps */3],
          /* didMount */(function (self) {
              var watchId = ReasonReact.Router[/* watchUrl */1]((function (url) {
                      return Curry._1(self[/* send */3], /* ChangeUrl */[Config$Workhours.urlToRoute(url)]);
                    }));
              return Curry._1(self[/* onUnmount */4], (function () {
                            return ReasonReact.Router[/* unwatchUrl */2](watchId);
                          }));
            }),
          /* didUpdate */component[/* didUpdate */5],
          /* willUnmount */component[/* willUnmount */6],
          /* willUpdate */component[/* willUpdate */7],
          /* shouldUpdate */component[/* shouldUpdate */8],
          /* render */(function (param) {
              return Curry._1(children, param[/* state */1][/* currentRoute */0]);
            }),
          /* initialState */(function () {
              return /* record */[/* currentRoute */Config$Workhours.urlToRoute(ReasonReact.Router[/* dangerouslyGetInitialUrl */3](/* () */0))];
            }),
          /* retainedProps */component[/* retainedProps */11],
          /* reducer */(function (action, _) {
              return /* Update */Block.__(0, [/* record */[/* currentRoute */action[0]]]);
            }),
          /* jsElementWrapped */component[/* jsElementWrapped */13]
        ];
}

var WithRouter = /* module */[
  /* component */component,
  /* make */make
];

var component$1 = ReasonReact.statelessComponent("Link");

function make$1(route, $staropt$star, children) {
  var className = $staropt$star !== undefined ? $staropt$star : "";
  return /* record */[
          /* debugName */component$1[/* debugName */0],
          /* reactClassInternal */component$1[/* reactClassInternal */1],
          /* handedOffState */component$1[/* handedOffState */2],
          /* willReceiveProps */component$1[/* willReceiveProps */3],
          /* didMount */component$1[/* didMount */4],
          /* didUpdate */component$1[/* didUpdate */5],
          /* willUnmount */component$1[/* willUnmount */6],
          /* willUpdate */component$1[/* willUpdate */7],
          /* shouldUpdate */component$1[/* shouldUpdate */8],
          /* render */(function (self) {
              var href = Config$Workhours.routeToString(route);
              return ReasonReact.createDomElement("a", {
                          className: className,
                          href: href,
                          onClick: Curry._1(self[/* handle */0], (function ($$event, _) {
                                  $$event.preventDefault();
                                  return ReasonReact.Router[/* push */0](href);
                                }))
                        }, children);
            }),
          /* initialState */component$1[/* initialState */10],
          /* retainedProps */component$1[/* retainedProps */11],
          /* reducer */component$1[/* reducer */12],
          /* jsElementWrapped */component$1[/* jsElementWrapped */13]
        ];
}

var Link = /* module */[
  /* component */component$1,
  /* make */make$1
];

var component$2 = ReasonReact.statelessComponent("NavLink");

function make$2(route, activeRoute, className, children) {
  return /* record */[
          /* debugName */component$2[/* debugName */0],
          /* reactClassInternal */component$2[/* reactClassInternal */1],
          /* handedOffState */component$2[/* handedOffState */2],
          /* willReceiveProps */component$2[/* willReceiveProps */3],
          /* didMount */component$2[/* didMount */4],
          /* didUpdate */component$2[/* didUpdate */5],
          /* willUnmount */component$2[/* willUnmount */6],
          /* willUpdate */component$2[/* willUpdate */7],
          /* shouldUpdate */component$2[/* shouldUpdate */8],
          /* render */(function () {
              return ReasonReact.element(undefined, undefined, make((function (currentRoute) {
                                var activeRoute$1 = activeRoute !== undefined ? activeRoute : route;
                                var className$1 = Cn.make(/* :: */[
                                      Cn.unpack(className),
                                      /* :: */[
                                        Cn.ifTrue("active", activeRoute$1 === currentRoute),
                                        /* [] */0
                                      ]
                                    ]);
                                return ReasonReact.element(undefined, undefined, make$1(route, className$1, children));
                              })));
            }),
          /* initialState */component$2[/* initialState */10],
          /* retainedProps */component$2[/* retainedProps */11],
          /* reducer */component$2[/* reducer */12],
          /* jsElementWrapped */component$2[/* jsElementWrapped */13]
        ];
}

var NavLink = /* module */[
  /* component */component$2,
  /* make */make$2
];

exports.WithRouter = WithRouter;
exports.Link = Link;
exports.NavLink = NavLink;
/* component Not a pure module */