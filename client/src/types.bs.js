// Generated by BUCKLESCRIPT VERSION 4.0.5, PLEASE EDIT WITH CARE
'use strict';

var Json_decode = require("@glennsl/bs-json/src/Json_decode.bs.js");
var Json_encode = require("@glennsl/bs-json/src/Json_encode.bs.js");

function work(json) {
  return /* record */[
          /* id */Json_decode.optional((function (param) {
                  return Json_decode.field("id", Json_decode.string, param);
                }), json),
          /* start */Json_decode.field("start", Json_decode.date, json),
          /* duration */Json_decode.field("duration", Json_decode.$$float, json),
          /* lunch */Json_decode.field("lunch", Json_decode.$$float, json),
          /* userid */Json_decode.optional((function (param) {
                  return Json_decode.field("userid", Json_decode.string, param);
                }), json)
        ];
}

function workList(json) {
  return Json_decode.array(work, json);
}

var Decode = /* module */[
  /* work */work,
  /* workList */workList
];

function work$1(w) {
  return Json_encode.object_(/* :: */[
              /* tuple */[
                "id",
                Json_encode.nullable((function (prim) {
                        return prim;
                      }), w[/* id */0])
              ],
              /* :: */[
                /* tuple */[
                  "start",
                  Json_encode.date(w[/* start */1])
                ],
                /* :: */[
                  /* tuple */[
                    "duration",
                    w[/* duration */2]
                  ],
                  /* :: */[
                    /* tuple */[
                      "lunch",
                      w[/* lunch */3]
                    ],
                    /* :: */[
                      /* tuple */[
                        "userid",
                        Json_encode.nullable((function (prim) {
                                return prim;
                              }), w[/* userid */4])
                      ],
                      /* [] */0
                    ]
                  ]
                ]
              ]
            ]);
}

var Encode = /* module */[/* work */work$1];

exports.Decode = Decode;
exports.Encode = Encode;
/* Json_encode Not a pure module */