open Jest;

open Js.Date;

test("add 2 days", (_) => {
  let startDate = makeWithYMD(~year=2018., ~month=7., ~date=17., ());
  let endDate = makeWithYMD(~year=2018., ~month=7., ~date=19., ());
  Expect.(
    expect(DateUtil.addDays(startDate, 2.) |> valueOf)
    |> toBe(endDate |> valueOf)
  );
});

test("subtract 2 days", (_) => {
  let startDate = makeWithYMD(~year=2018., ~month=7., ~date=17., ());
  let endDate = makeWithYMD(~year=2018., ~month=7., ~date=15., ());
  Expect.(
    expect(DateUtil.addDays(startDate, -2.) |> valueOf)
    |> toBe(endDate |> valueOf)
  );
});

test("add 2 days accross month", (_) => {
  let startDate = makeWithYMD(~year=2018., ~month=7., ~date=30., ());
  let endDate = makeWithYMD(~year=2018., ~month=8., ~date=1., ());
  Expect.(
    expect(DateUtil.addDays(startDate, 2.) |> valueOf)
    |> toBe(endDate |> valueOf)
  );
});

test("add 2 days accross year", (_) => {
  let startDate = makeWithYMD(~year=2018., ~month=12., ~date=30., ());
  let endDate = makeWithYMD(~year=2019., ~month=1., ~date=1., ());
  Expect.(
    expect(DateUtil.addDays(startDate, 2.) |> valueOf)
    |> toBe(endDate |> valueOf)
  );
});

test("Start of week", (_) => {
  let startDate = makeWithYMD(~year=2018., ~month=6., ~date=17., ());
  let endDate = makeWithYMD(~year=2018., ~month=6., ~date=15., ());
  Expect.(
    expect(DateUtil.startOfWeek(startDate) |> toDateString)
    |> toBe(endDate |> toDateString)
  );
});

test("Start of week on start of week", (_) => {
  let startDate = makeWithYMD(~year=2018., ~month=6., ~date=15., ());
  let endDate = makeWithYMD(~year=2018., ~month=6., ~date=15., ());
  Expect.(
    expect(DateUtil.startOfWeek(startDate) |> toDateString)
    |> toBe(endDate |> toDateString)
  );
});

test("Start of week on end of week", (_) => {
  let startDate = makeWithYMD(~year=2018., ~month=6., ~date=21., ());
  let endDate = makeWithYMD(~year=2018., ~month=6., ~date=15., ());
  Expect.(
    expect(DateUtil.startOfWeek(startDate) |> toDateString)
    |> toBe(endDate |> toDateString)
  );
});

test("End of week", (_) => {
  let startDate = makeWithYMD(~year=2018., ~month=6., ~date=17., ());
  let endDate =
    makeWithYMDHMS(
      ~year=2018.,
      ~month=6.,
      ~date=21.,
      ~hours=23.,
      ~minutes=59.,
      ~seconds=59.,
      (),
    );
  setMilliseconds(endDate, 999.) |> ignore;
  Expect.(
    expect(DateUtil.endOfWeek(startDate) |> Js.Date.toDateString)
    |> toBe(endDate |> toDateString)
  );
});

test("End of week on end of week", (_) => {
  let startDate = makeWithYMD(~year=2018., ~month=6., ~date=21., ());
  let endDate =
    makeWithYMDHMS(
      ~year=2018.,
      ~month=6.,
      ~date=21.,
      ~hours=23.,
      ~minutes=59.,
      ~seconds=59.,
      (),
    );
  setMilliseconds(endDate, 999.) |> ignore;
  Expect.(
    expect(DateUtil.endOfWeek(startDate) |> Js.Date.toDateString)
    |> toBe(endDate |> toDateString)
  );
});

test("End of week on start of week", (_) => {
  let startDate = makeWithYMD(~year=2018., ~month=6., ~date=15., ());
  let endDate =
    makeWithYMDHMS(
      ~year=2018.,
      ~month=6.,
      ~date=21.,
      ~hours=23.,
      ~minutes=59.,
      ~seconds=59.,
      (),
    );
  setMilliseconds(endDate, 999.) |> ignore;
  Expect.(
    expect(DateUtil.endOfWeek(startDate) |> Js.Date.toDateString)
    |> toBe(endDate |> toDateString)
  );
});