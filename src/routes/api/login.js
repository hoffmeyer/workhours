// @flow

import {
    Router
} from 'express';
import {
    isLoggedIn
} from '../../util/auth';
import passport from 'passport';

let router = Router();

router.post('/', passport.authenticate('basic'), (req, res) => res.send(req.user));

export default router;