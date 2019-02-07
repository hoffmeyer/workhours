import { Router } from 'express';
import * as passport from 'passport';

let router = Router();

router.post('/', passport.authenticate('login'), (req, res) => res.send(req.user));

export default router;