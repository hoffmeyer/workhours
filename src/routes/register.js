// @flow
import {Router} from 'express';
import type {Work} from '../types';

let router = Router();

router.post('/', function(req, res, next) {
        let work : Work = req.body;
        console.log(work);
});

export default router;
